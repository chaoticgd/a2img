/*
	a2img - a free and open source image editor
	Copyright (C) 2018 chaoticgd

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "canvas.h"

#include <tgmath.h>

#include <QPainter>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QDateTime>

#include "application.h"
#include "util/rectangle.h"
#include "glbuffer.h"

a2img::Canvas::Canvas(Application* app)
	: app_(app),
	  zoomPercentage_(100),
	  lastViewportRectangle_(0, 0, 0, 0)
{
	app_->canvas()->draw([=](GlFunctions* gl) {

		A2_GL_ENTER(gl)

		// Load shader to draw the chequerboard background
		// for transparent images.
		chequerBoardShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/chequerboard_vertex.glsl");
		chequerBoardShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/chequerboard_fragment.glsl");
		chequerBoardShader_.link();

		// Load the shader to draw the image normally.
		textureShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/texture_vertex.glsl");
		textureShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/texture_fragment.glsl");
		textureShader_.link();

		// Load the shader to draw a selection area.
		selectionShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/texture_vertex.glsl");
		selectionShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/selection_fragment.glsl");
		selectionShader_.link();

		A2_GL_EXIT(gl)
	});

	liveUpdateTimer_.setInterval(10);
	QObject::connect(&liveUpdateTimer_, &QTimer::timeout, [=]() {
		update(RedrawMode::onlyRedrawForeground);
	});

	liveUpdateTimer_.start();
}

void a2img::Canvas::update(RedrawMode mode)
{
	if(mode == RedrawMode::fullRedraw) {
		canvasCache_.reset(nullptr);
	}

	QGraphicsScene::update();
}

void a2img::Canvas::draw(DrawCallback callback)
{
	// Adds the draw request to the request queue.
	drawRequests_.push_back(callback);
	update(RedrawMode::onlyRedrawForeground);
}

float a2img::Canvas::getZoomPercentage()
{
	return static_cast<float>(zoomPercentage_);
}

void a2img::Canvas::setZoomPercentage(float newZoomPercentage)
{
	float oldZoomPercentage = zoomPercentage_;

	// Animate the zoom using QTimer.
	for(int i = 0; i <= 4; i++) {
		float step = static_cast<float>(i) / 4;
		QTimer::singleShot(step * 50, [=]() {
			zoomPercentage_ = oldZoomPercentage *(1 - step) + newZoomPercentage * step;
			update(RedrawMode::fullRedraw);
		});
	}
}

QRectF a2img::Canvas::getViewportRectangle()
{
	return lastViewportRectangle_;
}

void a2img::Canvas::beginLiveUpdates()
{
	liveUpdateTimer_.start();
}

void a2img::Canvas::endLiveUpdates()
{
	liveUpdateTimer_.stop();
}

void a2img::Canvas::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{

}

void a2img::Canvas::dropEvent(QGraphicsSceneDragDropEvent* event)
{

}

void a2img::Canvas::drawBackground(QPainter* painter, const QRectF& rect)
{
	lastViewportRectangle_ = rect;

	gl_.initializeOpenGLFunctions();

	A2_GL_ENTER(&gl_);

	painter->beginNativePainting();

	// Handle all pending draw requests.
	handleDrawRequests();

	//
	// Render the image to the QGraphicsView.
	//

	// Setup the frame buffer again.
	// Only a problem on Windows and/or with older versions of Qt.
	painter->beginNativePainting();

	if(app_->window()->isDarkThemeInUse()) {
		gl_.glClearColor(0.5f, 0.5f, 0.5f, 1);
	} else {
		gl_.glClearColor(0.8f, 0.8f, 0.8f, 1);
	}
	gl_.glClear(GL_COLOR_BUFFER_BIT);

	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}


	Document* doc = app_->documentManager().currentDocument();
	float zoom = zoomPercentage_ / 100.0;
	QSize documentSize = doc->size() * zoom;
	setSceneRect(0, 0, documentSize.width(), documentSize.height());

	GLuint textureId = renderLayersToTexture();

	// If the image is small, render it smoothly.
	// Otherwise, show defined pixel boundaries.
	if(zoom < 1) {
		gl_.glBindTexture(GL_TEXTURE_2D, textureId);
		gl_.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl_.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	drawDocument(textureId, rect);
	painter->endNativePainting();

	// Cleaup.
	gl_.glDeleteTextures(1, &textureId);

	A2_GL_EXIT(&gl_)
}

void a2img::Canvas::handleDrawRequests()
{
	A2_GL_ENTER(&gl_)

	while(drawRequests_.size() > 0) {
		DrawCallback request = *drawRequests_.begin();
		drawRequests_.pop_front();
		request(&gl_);
	}

	A2_GL_EXIT(&gl_)
}

GLuint a2img::Canvas::renderLayersToTexture()
{
	Document* doc = app_->documentManager().currentDocument();
	QOpenGLFramebufferObject fbo(doc->size());
	fbo.bind();
	gl_.glViewport(0, 0, doc->size().width(), doc->size().height());
	gl_.glClearColor(0, 0, 0, 0);
	gl_.glClear(GL_COLOR_BUFFER_BIT);
	doc->rootLayer()->draw(&gl_, fbo, true);
	return fbo.takeTexture();
}

void a2img::Canvas::drawDocument(GLuint layersTexture, QRectF rect)
{
	A2_GL_ENTER(&gl_)
	Document* doc = app_->documentManager().currentDocument();
	float zoom = zoomPercentage_ / 100.0;
	QSize documentSize = doc->size() * zoom;

	// Setup scene.
	gl_.glViewport(0, 0, rect.size().width(), rect.size().height());

	// Convert from Qt coordinates to OpenGL coordinates.
	double yPos = documentSize.height() - rect.bottom();
	Rectangle<float> viewport(
		rect.x(),
		yPos,
		rect.x() + rect.width(),
		yPos + rect.height()
	);

	// Vertex Data

	Rectangle<float> vertexRect {
		0,
		0,
		doc->size().width() * zoom,
		doc->size().height() * zoom
	};
	std::array<float, 18> vertexData = vertexRect.toScreenSpace(viewport).vertexData();
	VertexBuffer vertexBuffer(&gl_, vertexData);
	DefaultUvBuffer uvBuffer(&gl_);

	auto drawArrays = [=, &vertexBuffer, &uvBuffer]() {
		gl_.drawArrays(vertexBuffer, uvBuffer);
	};
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawChequerBoard(rect.topLeft(), drawArrays);
	drawLayers(layersTexture, drawArrays);
	drawSelectionOverlay(drawArrays);
	glDisable(GL_BLEND);

	A2_GL_EXIT(&gl_)
}

void a2img::Canvas::drawChequerBoard(QPointF scrollPosition, std::function<void()> drawArrays)
{
	A2_GL_ENTER(&gl_)

	chequerBoardShader_.bind();

	GLint scrollPositionLoc = chequerBoardShader_.uniformLocation("scrollPosition");
	gl_.glUniform2f(scrollPositionLoc, scrollPosition.x(), scrollPosition.y());

	GLint tileSizeLoc = chequerBoardShader_.uniformLocation("tileSize");
	gl_.glUniform1i(tileSizeLoc, 8);

	drawArrays();

	A2_GL_EXIT(&gl_)
}

void a2img::Canvas::drawLayers(GLuint layersTexture, std::function<void()> drawArrays)
{
	A2_GL_ENTER(&gl_)

	textureShader_.bind();


	// Texture
	gl_.glActiveTexture(GL_TEXTURE0);
	gl_.glBindTexture(GL_TEXTURE_2D, layersTexture);

	GLint location = textureShader_.uniformLocation("texture");
	gl_.glUniform1i(location, 0);

	// Draw Call

	drawArrays();

	A2_GL_EXIT(&gl_)
}

void a2img::Canvas::drawSelectionOverlay(std::function<void()> drawArrays)
{
	A2_GL_ENTER(&gl_)

	Document* doc = app_->documentManager().currentDocument();

	if(!doc->isRegionSelected()) {
		return;
	}

	selectionShader_.bind();

	// Texture
	doc->selectionMask().bind(&gl_, 0, TextureQuality::pixelated);

	GLint maskLocation = selectionShader_.uniformLocation("mask");
	gl_.glUniform1i(maskLocation, 0);


	GLint sizeLocation = selectionShader_.uniformLocation("viewportSize");
	float zoom = getZoomPercentage() / 100;
	gl_.glUniform2f(sizeLocation, doc->size().width() * zoom, doc->size().height() * zoom);

	GLint timeLocation = selectionShader_.uniformLocation("time");
	gl_.glUniform1f(timeLocation, (QDateTime::currentMSecsSinceEpoch() % 256) / 32);

	// Draw Call
	drawArrays();

	A2_GL_EXIT(&gl_)
}

void a2img::Canvas::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}
	Document* doc = app_->documentManager().currentDocument();

	// Forward the event to children.
	QGraphicsScene::mouseMoveEvent(event);


	// Set the "Mouse Position" status bar label.
	float zoom = getZoomPercentage() / 100;
	QPointF flooredPos {
		floor(event->scenePos().x() / zoom),
		floor(event->scenePos().y() / zoom)
	};
	QPoint qtPosition = (flooredPos).toPoint();
	// Correct the Y coordinate so up is positive.
	QPoint documentPosition {
		qtPosition.x(),
		doc->size().height() - qtPosition.y()
	};
	app_->window()->setStatusMousePosition(documentPosition);
}

void a2img::Canvas::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent)
{
	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}
	Document* doc = app_->documentManager().currentDocument();

	float zoom = getZoomPercentage() / 100;
	QSize size = doc->size() * zoom;
	QPointF offset = wheelEvent->scenePos();

	if(wheelEvent->delta() > 0) {
		setZoomPercentage(getZoomPercentage() * 1.25F);
	} else {
		setZoomPercentage(getZoomPercentage() / 1.25F);
	}
}

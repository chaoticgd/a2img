#include "layer.h"

#include <algorithm>
#include <utility>

#include "application.h"
#include "document/document.h"
#include "util/rectangle.h"
#include "vectorutil.h"
#include "glbuffer.h"

a2img::Layer::Layer(Application* app, Document* document, QString name)
	: app_(app),
	  document_(document),
	  name_(name),
	  texture_(app),
	  isVisible_(true),
	  thumbnail_(32, 32)
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		texture_ = Texture(app, gl, document->size());

		textureShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/texture_vertex.glsl");
		textureShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/texture_fragment.glsl");
		textureShader_.link();

		chequerBoardShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/chequerboard_vertex.glsl");
		chequerBoardShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/chequerboard_fragment.glsl");
		chequerBoardShader_.link();
	});
}

void a2img::Layer::create(QSize size, QColor backgroundColour)
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		QOpenGLFramebufferObject fbo(size);
		fbo.bind();

		// Apply the background colour.
		gl->glViewport(0, 0, size.width(), size.height());
		gl->glClearColor(
			backgroundColour.red() / 255.0F,
			backgroundColour.green() / 255.0F,
			backgroundColour.blue() / 255.0F,
			backgroundColour.alpha() / 255.0F
		);
		gl->glClear(GL_COLOR_BUFFER_BIT);

		// Replace the default texture.
		texture_ = Texture(app_, fbo.takeTexture(), size);
	});

	updateThumbnail();
	document_->registerLayer(this);
}

void a2img::Layer::create(QImage image)
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		texture_ = Texture(app_, gl, image);
	});

	updateThumbnail();
	document_->registerLayer(this);
}

a2img::Application* a2img::Layer::app()
{
	return app_;
}

a2img::Document* a2img::Layer::document()
{
	return document_;
}

QString a2img::Layer::displayName()
{
	return name_;
}

void a2img::Layer::setDisplayName(QString name)
{
	name_ = name;
}

a2img::LayerId a2img::Layer::id()
{
	return document_->idFromLayer(this);
}

QSize a2img::Layer::size()
{
	return texture_.size();
}

GLuint a2img::Layer::texture()
{
	return texture_.id();
}

void a2img::Layer::replaceTexture(QUndoCommand*, std::function<Texture(Texture& in)> callback)
{
	texture_ = callback(texture_);
	updateThumbnail();
}

std::vector<a2img::Texture*> a2img::Layer::maps()
{
	return { &texture_ };
}

void a2img::Layer::draw(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview)
{
	if(!isVisible_) {
		return;
	}

	drawSelf(gl, fbo, preview);
	drawChildren(gl, fbo, preview);
}

void a2img::Layer::drawSelf(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview)
{
	A2_GL_ENTER(gl)

	if(!texture_.isValid() || parent() == nullptr) {
		return;
	}

	Texture* intermediateTexture = &texture_;

	if(preview) {
		// Apply the previewed command (e.g. a brush stroke the user is currently drawing).
		if(document_->selectedLayer() == this) {
			TextureStorageCommand* previewCommand = app_->documentManager().getPreviewCommand();
			if(previewCommand != nullptr) {
				Texture newTexture(previewCommand->renderToTexture(gl, texture_, true));
				intermediateTexture = &newTexture;
			}
		}

		// Apply graphical effects.
		for(std::unique_ptr<TextureStorageCommand>& command : queuedCommands_) {
			Texture newTexture(command->renderToTexture(gl, *intermediateTexture, true));
			intermediateTexture = &newTexture;
		}
	}

	// Apply selection mask. TODO: ABACUS
	/*Layer* selectionMask = document_->selectionMaskLayer();
	GLuint texture = intermediateTexture;
	if(document_->isRegionSelected()) {
		texture = app_->maskManager().applyMask(gl, texture, texture_->id(), selectionMask->texture());
		if(intermediateTexture != texture_->id()) {
			gl->glDeleteTextures(1, &intermediateTexture);
		}
	}*/

	fbo.bind();
	textureShader_.bind();

	gl->glActiveTexture(GL_TEXTURE0);
	gl->glBindTexture(GL_TEXTURE_2D, intermediateTexture->id());

	DefaultVertexBuffer vertexBuffer(gl);
	DefaultUvBuffer uvBuffer(gl);

	// Draw Call
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl->drawArrays(vertexBuffer, uvBuffer);
	glDisable(GL_BLEND);

	A2_GL_EXIT(gl)
}

void a2img::Layer::drawChildren(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview)
{
	for(Layer* child : children()) {
		child->draw(gl, fbo, preview);
	}
}

void a2img::Layer::addQueuedCommand(std::unique_ptr<TextureStorageCommand> command)
{
	queuedCommands_.push_back(std::move(command));
	app_->effectManager().updatePanel();
	app_->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::Layer::removeQueuedCommand(TextureStorageCommand* effect)
{
	for(auto iter = queuedCommands_.begin(); iter < queuedCommands_.end(); iter++) {
		if(iter->get() == effect) {
			queuedCommands_.erase(iter);
			app_->effectManager().updatePanel();
			app_->canvas()->update(RedrawMode::fullRedraw);
			return;
		}
	}

	qWarning() << "Tried to remove a pending effect" << effect << "that doesn't exist.";
}

void a2img::Layer::moveQueuedCommand(TextureStorageCommand* effect, bool towardsEnd)
{
	auto originalIter = std::find_if(
		queuedCommands_.begin(), queuedCommands_.end(),
		[=](std::unique_ptr<TextureStorageCommand>& command) {
			return command.get() == effect;
		}
	);

	if(originalIter == queuedCommands_.end()) {
		qWarning() << "Tried to move pending effect that does not exist.";
		return;
	}

	if(!towardsEnd && originalIter == queuedCommands_.begin()) {
		// No warning since this could happen normally.
		return;
	}

	auto otherIter = originalIter + (towardsEnd ? 1 : -1);

	if(otherIter == queuedCommands_.end()) {
		// No warning since this could happen normally.
		return;
	}

	originalIter->swap(*otherIter);
	app_->effectManager().updatePanel();
	app_->canvas()->update(RedrawMode::fullRedraw);
}

std::vector<a2img::TextureStorageCommand*> a2img::Layer::queuedCommands()
{
	return uniqueVectorToRaw<TextureStorageCommand>(queuedCommands_);
}

void a2img::Layer::applyEffects()
{
	for(std::unique_ptr<TextureStorageCommand>& effect : queuedCommands_) {
		document_->pushUndoCommand(effect.release());
	}
	queuedCommands_.clear();
	app_->effectManager().updatePanel();
}

bool a2img::Layer::isVisible()
{
	return isVisible_;
}

void a2img::Layer::setVisible(bool isVisible)
{
	isVisible_ = isVisible;
	app_->canvas()->update(RedrawMode::fullRedraw);
}

QPixmap a2img::Layer::thumbnail()
{
	return thumbnail_;
}

void a2img::Layer::updateThumbnail()
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		QOpenGLFramebufferObject thumbnailFbo(32, 32);
		thumbnailFbo.bind();

		gl->glViewport(0, 0, 32, 32);

		DefaultVertexBuffer vertexBuffer(gl);
		DefaultUvBuffer uvBuffer(gl);

		// Draw chequer board.
		chequerBoardShader_.bind();
		GLint tileSizeLoc = chequerBoardShader_.uniformLocation("tileSize");
		gl->glUniform1i(tileSizeLoc, 4);
		gl->drawArrays(vertexBuffer, uvBuffer);

		// Draw foreground.
		texture_.bind(gl, 0, TextureQuality::smooth);

		textureShader_.bind();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gl->drawArrays(vertexBuffer, uvBuffer);
		glDisable(GL_BLEND);

		thumbnail_ = QPixmap::fromImage(thumbnailFbo.toImage());

		// Update the model.
		LayerModel* model = document_->layerModel();
		QModelIndex thisIndex = model->indexFromLayer(this);
		document_->layerModel()->dataChanged(thisIndex, thisIndex);
	});
}

QJsonObject a2img::Layer::serialiseMetadata()
{
	QJsonObject result;
	result.insert("name", QJsonValue(name_));
	result.insert("id", id());

	QString colourMapPath = "images/" + QString::number(reinterpret_cast<long>(&texture_), 16) + ".png";
	result.insert("colourMap", QJsonValue(colourMapPath));

	QJsonArray childrenValue;
	for(Layer* child : children()) {
		childrenValue.append(child->serialiseMetadata());
	}
	result.insert("children", childrenValue);

	return result;
}

a2img::LayerId a2img::Layer::invalidId = 0;

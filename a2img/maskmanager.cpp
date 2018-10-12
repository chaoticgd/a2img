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

#include "maskmanager.h"

#include "application.h"
#include "document/document.h"
#include "util/rectangle.h"
#include "glbuffer.h"

a2img::MaskManager::MaskManager(Application* app)
	: app_(app)
{
	loadShaders();
}

a2img::Texture a2img::MaskManager::createMask(GlFunctions* gl, Vec2<int> size, Rectangle<int> foregroundRectangle)
{
	QOpenGLFramebufferObject fbo(size.toQSize());
	fbo.bind();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	Rectangle<float> viewport {
		{ 0, 0 }, size
	};

	VertexBuffer vertexBuffer {
		gl,
		Rectangle<float>(foregroundRectangle).toScreenSpace(viewport).vertexData()
	};

	whiteShader_.bind();
	gl->drawArrays(vertexBuffer);

	return Texture(app_, fbo);
}

a2img::Texture a2img::MaskManager::applyMask(GlFunctions* gl, QColor foreground, const Texture& background, const Texture& mask, BlendMode blendMode)
{
	A2_GL_ENTER(gl)

	Document* doc = app_->documentManager().currentDocument();

	// Setup frame buffer for drawing output.
	QOpenGLFramebufferObject fbo(doc->size());
	fbo.bind();

	gl->glViewport(0, 0, doc->size().width(), doc->size().height());
	gl->glClearColor(0, 0, 0, 0);
	gl->glClear(GL_COLOR_BUFFER_BIT);

	QOpenGLShaderProgram* currentShader;
	switch(blendMode) {
	case BlendMode::normal:
		currentShader = &colourShader_;
		break;
	case BlendMode::multiply:
		// Used for the highlighter pen.
		currentShader = &colourMultiplyShader_;
		break;
	}
	currentShader->bind();

	// Texture

	background.bind(gl, 0, TextureQuality::pixelated);
	mask.bind(gl, 1, TextureQuality::pixelated);

	GLint backgroundLocation = currentShader->uniformLocation("backgroundSampler");
	gl->glUniform1i(backgroundLocation, 0);

	GLint maskLocation = currentShader->uniformLocation("maskSampler");
	gl->glUniform1i(maskLocation, 1);

	GLint resolutionLocation = currentShader->uniformLocation("resolution");
	gl->glUniform2f(resolutionLocation, doc->size().width(), doc->size().height());

	// Colour

	GLint brushColourLocation = currentShader->uniformLocation("brushColour");
	gl->glUniform4f(
		brushColourLocation,
		foreground.redF(),
		foreground.greenF(),
		foreground.blueF(),
		foreground.alphaF()
	);

	// Draw

	DefaultVertexBuffer vertexBuffer(gl);
	gl->drawArrays(vertexBuffer);

	// Cleanup

	A2_GL_EXIT(gl)

	return Texture(app_, fbo);
}

a2img::Texture a2img::MaskManager::applyMask(GlFunctions* gl, const Texture& foreground, const Texture& background, const Texture& mask)
{
	A2_GL_ENTER(gl)

	Document* doc = app_->documentManager().currentDocument();

	// Setup frame buffer.
	QOpenGLFramebufferObject fbo(doc->size());
	fbo.bind();

	gl->glViewport(0, 0, doc->size().width(), doc->size().height());
	gl->glClearColor(0, 0, 0, 0);
	gl->glClear(GL_COLOR_BUFFER_BIT);

	maskTextureShader_.bind();

	// Texture
	foreground.bind(gl, 0, TextureQuality::pixelated);
	background.bind(gl, 1, TextureQuality::pixelated);
	mask.bind(gl, 2, TextureQuality::pixelated);

	GLint foregroundLocation = maskTextureShader_.uniformLocation("foregroundSampler");
	gl->glUniform1i(foregroundLocation, 0);

	GLint backgroundLocation = maskTextureShader_.uniformLocation("backgroundSampler");
	gl->glUniform1i(backgroundLocation, 1);

	GLint maskLocation = maskTextureShader_.uniformLocation("maskSampler");
	gl->glUniform1i(maskLocation, 2);

	// Draw

	DefaultVertexBuffer vertexBuffer(gl);
	DefaultUvBuffer uvBuffer(gl);
	gl->drawArrays(vertexBuffer, uvBuffer);

	A2_GL_EXIT(gl)

	return Texture(app_, fbo);
}

void a2img::MaskManager::loadShaders()
{
	app_->canvas()->draw([=](GlFunctions*) {
		// Colour mask.

		colourShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colour_mask_vertex.glsl");
		colourShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colour_mask_fragment.glsl");
		colourShader_.bindAttributeLocation("position", 0);
		colourShader_.link();

		colourMultiplyShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/colour_mask_vertex.glsl");
		colourMultiplyShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/colour_mask_multiply_fragment.glsl");
		colourMultiplyShader_.bindAttributeLocation("position", 0);
		colourMultiplyShader_.link();

		// Texture mask.

		maskTextureShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/mask_texture_vertex.glsl");
		maskTextureShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/mask_texture_fragment.glsl");
		maskTextureShader_.bindAttributeLocation("position", 0);
		maskTextureShader_.link();

		// White shader.

		whiteShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/brush_segment_vertex.glsl");
		whiteShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/brush_segment_fragment.glsl");
		whiteShader_.bindAttributeLocation("position", 0);
		whiteShader_.link();
	});
}

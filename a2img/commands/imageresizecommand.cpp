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

#include "imageresizecommand.h"

#include "application.h"
#include "document/document.h"
#include "util/serialisation.h"
#include "glbuffer.h"

a2img::ImageResizeCommand::ImageResizeCommand(Application* app, Document* document, int newWidth, int newHeight, bool stretch, bool smooth)
    : app_(app),
      document_(document),
      newSize_(newWidth, newHeight),
      stretch_(stretch),
      smooth_(smooth),
      oldSize_(0, 0)
{
	app->canvas()->draw([=](GlFunctions* gl) {
		textureShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/texture_vertex.glsl");
		textureShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/texture_fragment.glsl");
		textureShader_.link();
	});
}

void a2img::ImageResizeCommand::redo()
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		oldSize_ = Vec2<int>(document_->size());
		document_->setSize(this, newSize_.toQSize());

		// Resize each layer in the image.
		for(Layer* layer : document_->layers()) {
			layer->replaceTexture(this, [=](Texture& in) {

				Rectangle<float> viewport(0, 0, newSize_.x, newSize_.y);
				QOpenGLFramebufferObject fbo(newSize_.x, newSize_.y);
				fbo.bind();

				gl->glViewport(0, 0, newSize_.x, newSize_.y);

				gl->glClearColor(0, 0, 0, 0);
				gl->glClear(GL_COLOR_BUFFER_BIT);

				textureShader_.bind();

				in.bind(gl, 0, smooth_ ? TextureQuality::smooth : TextureQuality::pixelated);

				GLint samplerLocation = textureShader_.uniformLocation("texture");
				gl->glUniform1i(samplerLocation, 0);

				DefaultUvBuffer uvBuffer(gl);
				if(stretch_) {
					// Cover the entire viewport.
					DefaultVertexBuffer stretchedVertexBuffer(gl);
					gl->drawArrays(stretchedVertexBuffer, uvBuffer);
				} else {
					// Pad the new space with transparency.
					Rectangle<float> vertexRectangle(0, 0, in.size().width(), in.size().height());
					VertexBuffer paddedVertexBuffer(gl, vertexRectangle.toScreenSpace(viewport).vertexData());
					gl->drawArrays(paddedVertexBuffer, uvBuffer);
				}

				// Move the old texture, so the operation can be undone later.
				oldTextures_.push_back(std::move(in));

				return Texture(app_, fbo);
			});
		}
	});
}

void a2img::ImageResizeCommand::undo()
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		document_->setSize(this, oldSize_.toQSize());

		for(size_t i = 0; i < document_->layers().size(); i++) {
			Layer* layer = document_->layers()[i];
			layer->replaceTexture(this, [=](Texture& in) {
				return std::move(oldTextures_[i]);
			});
		}
	});
}

QString a2img::ImageResizeCommand::type() const
{
	return "resizeImage";
}

void a2img::ImageResizeCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	metadata.insert("newSize", serialisation::toJsonObject(newSize_));
	metadata.insert("stretch", stretch_);
	metadata.insert("smooth", smooth_);

	metadata.insert("oldSize", serialisation::toJsonObject(oldSize_));
	for(const Texture& texture : oldTextures_) {
		storage.addTexture(&texture);
	}
}

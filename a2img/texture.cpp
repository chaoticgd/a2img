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

#include "texture.h"

#include "application.h"

a2img::Texture::Texture(Application* app)
	: app_(app),
	  textureId_(0),
	  size_({ 0, 0 })
{}

a2img::Texture::Texture(Application* app, GlFunctions* gl, QSize size)
	: app_(app),
	  size_(size)
{
	gl->glGenTextures(1, &textureId_);
	gl->glBindTexture(GL_TEXTURE_2D, textureId_);
}

a2img::Texture::Texture(Application* app, GLuint id, QSize size)
	: app_(app),
	  textureId_(id),
	  size_(size) {}

a2img::Texture::Texture(Application* app, QOpenGLFramebufferObject& fbo)
	: app_(app),
	  textureId_(fbo.takeTexture()),
	  size_(fbo.size()) {}

a2img::Texture::Texture(Application* app, GlFunctions* gl, QImage image)
	: app_(app),
	  size_(image.size())
{
	A2_GL_ENTER(gl)

	// QImage's store image data vertically flipped compared to OpenGL textures.
	QImage mirrored = image.mirrored();
	gl->glGenTextures(1, &textureId_);
	gl->glBindTexture(GL_TEXTURE_2D, textureId_);

	// Copy the image data into the texture.
	gl->glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		mirrored.width(), mirrored.height(), 0,
		GL_BGRA, GL_UNSIGNED_BYTE, mirrored.bits()
	);

	A2_GL_EXIT(gl)
}

a2img::Texture::Texture(Texture&& rhs)
	: app_(rhs.app_),
	  textureId_(rhs.textureId_),
	  size_(rhs.size_)
{
	rhs.textureId_ = 0;
}

a2img::Texture::~Texture()
{
	GLuint textureToDelete = textureId_;
	app_->canvas()->draw([=](GlFunctions* gl) {
		gl->glDeleteTextures(1, &textureToDelete);
	});
}

a2img::Texture& a2img::Texture::operator=(Texture rhs)
{
	app_ = rhs.app_;
	textureId_ = rhs.textureId_;
	size_ = rhs.size_;

	rhs.textureId_ = 0;
	return *this;
}

GLuint a2img::Texture::id()
{
	return textureId_;
}

GLuint a2img::Texture::takeId()
{
	GLuint id = textureId_;
	textureId_ = 0;
	return id;
}

QSize a2img::Texture::size() const
{
	return size_;
}

bool a2img::Texture::isValid() const
{
	return textureId_ != 0;
}

void a2img::Texture::bind(GlFunctions* gl, int n, TextureQuality quality) const
{
	gl->glActiveTexture(GL_TEXTURE0 + n);
	gl->glBindTexture(GL_TEXTURE_2D, textureId_);

	gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(quality));
	gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(quality));
}

QImage a2img::Texture::toQImage(GlFunctions* gl) const
{
	std::vector<uchar> buffer(static_cast<size_t>(size_.width() * size_.height()) * 4);
	bind(gl, 0, TextureQuality::pixelated);
	gl->glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
	QImage result(buffer.data(), size_.width(), size_.height(), QImage::Format_RGBA8888);
	return result.mirrored();
}

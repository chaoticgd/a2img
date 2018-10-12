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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <QSize>
#include <QImage>
#include <QOpenGLFramebufferObject>

#include "glfunctions.h"

namespace a2img {
	class Application;

	enum class TextureQuality {
		smooth = GL_LINEAR,
		pixelated = GL_NEAREST
	};

	// Owns an OpenGL texture.
	class Texture
	{
	public:
		// Creates an invalid texture.
		Texture(Application* app);

		// Creates a new OpenGL texture of the specified size.
		Texture(Application* app, GlFunctions* gl, QSize size);

		// Takes ownership of an existing OpenGL texture.
		Texture(Application* app, GLuint id, QSize size);

		// Takes ownership of the colour buffer of an FBO.
		Texture(Application* app, QOpenGLFramebufferObject& fbo);

		// Creates a new OpenGL texture from a QImage.
		// note: The contents of the image is flipped vertically as this is how Qt loads/saves images.
		Texture(Application* app, GlFunctions* gl, QImage image);

		// Move constructor.
		Texture(Texture&& rhs);

		// Deletes the OpenGL texture.
		~Texture();

		Texture& operator=(Texture rhs);

		// Returns the texture ID.
		GLuint id();

		// Returns and takes ownership of the texture ID.
		// This invalidates the object.
		GLuint takeId();

		// Returns the size of the texture.
		QSize size() const;

		// Returns whether or not the texture ID is zero.
		bool isValid() const;

		// Bind the current texture.
		void bind(GlFunctions* gl, int n, TextureQuality quality) const;

		// Copy the data into a QImage.
		QImage toQImage(GlFunctions* gl) const;

	private:
		// Disable the copy constructor.
		Texture(const Texture&);

		Application* app_;

		GLuint textureId_;
		QSize size_;
	};
}

#endif // TEXTURE_H

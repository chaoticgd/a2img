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

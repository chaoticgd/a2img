#ifndef MASKMANAGER_H
#define MASKMANAGER_H

#include <QColor>
#include <QOpenGLShaderProgram>

#include "glfunctions.h"
#include "texture.h"
#include "util/rectangle.h"

namespace a2img {
	class Application;

	enum BlendMode {
		normal = 0,
		multiply = 1
	};

	// Manages shaders related to the image masking and merging.
	class MaskManager
	{
	public:
		MaskManager(Application* app);

		Texture createMask(GlFunctions* gl, Vec2<int> size, Rectangle<int> foregroundRectangle);

		// Applies a mask to the in texture, returns the result.
		Texture applyMask(GlFunctions* gl, QColor foreground, const Texture& background, const Texture& mask, BlendMode blendMode);

		// Applies a mask to the background, returns the result.
		Texture applyMask(GlFunctions* gl, const Texture& foreground, const Texture& background, const Texture& mask);

	protected:
		void loadShaders();

	private:
		Application* app_;

		QOpenGLShaderProgram colourShader_;
		QOpenGLShaderProgram colourMultiplyShader_;
		QOpenGLShaderProgram maskTextureShader_;
		QOpenGLShaderProgram whiteShader_;
	};
}

#endif // MASKMANAGER_H

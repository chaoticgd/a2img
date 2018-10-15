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

	private:
		Application* app_;

		QOpenGLShaderProgram colourShader_;
		QOpenGLShaderProgram colourMultiplyShader_;
		QOpenGLShaderProgram maskTextureShader_;
		QOpenGLShaderProgram whiteShader_;
	};
}

#endif // MASKMANAGER_H

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

#ifndef EFFECT_H
#define EFFECT_H

#include <memory>
#include <string>
#include <vector>

#include <QOpenGLShaderProgram>

#include "glfunctions.h"
#include "effectproperty.h"
#include "texture.h"

#define EFFECTS_DIRECTORY "../effects"

namespace a2img {
	class Layer;

	enum class WrapMode
	{
		repeat = GL_REPEAT,
		mirroredRepeat = GL_MIRRORED_REPEAT,
		clampToEdge = GL_CLAMP_TO_EDGE,
		clampToBorder = GL_CLAMP_TO_BORDER
	};

	struct EffectStage
	{
		QString vertexPath;
		QString fragmentPath;
		QOpenGLShaderProgram* shader;
		WrapMode wrapMode;
	};

	class InvalidEffectException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class Effect
	{
	public:
		Effect(Application* app, QString package, QString name);
		~Effect();

		QString package();
		QString name();

		QString category();
		QString displayName();

		QString relativePath();

		std::vector<EffectProperty*> properties();

		// Compile and link shader stages.
		// Requires valid OpenGL context.
		void compileShaders();

		// Applies the effect to the input texture.
		// returns: The result.
		GLuint draw(GlFunctions* gl, const Texture& in, QSize size, std::vector<a2img::EffectProperty*> properties);

	protected:
		void loadManifest();
		void loadStage(QJsonObject stageObject);
		void loadProperty(QJsonObject propertyObject);

		QString processSourceFile(QString glslFilePath, bool isTopLevel);
		QString processSource(QString glslSourceCode);
		static QString insertIncludeGuard(QString filePath, QString sourceCode);

	private:
		Application* app_;

		QString package_;
		QString name_;

		QString category_;
		QString displayName_;

		std::vector<EffectStage> stages_;
		std::vector<std::unique_ptr<EffectProperty>> templateProperties_;
	};
}

#endif // EFFECT_H

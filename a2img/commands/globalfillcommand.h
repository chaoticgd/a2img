#ifndef GLOBALFILLCOMMAND_H
#define GLOBALFILLCOMMAND_H

#include <QOpenGLShaderProgram>
#include <QColor>

#include "fillcommand.h"

namespace a2img {
	class Application;

	class GlobalFillCommand : public FillCommand
	{
	public:
		GlobalFillCommand(Layer* layer, QColor colour, int tolerance);

		Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview);

	protected:
		void loadShaders();

	private:
		QOpenGLShaderProgram shader_;
	};
}

#endif // GLOBALFILLCOMMAND_H

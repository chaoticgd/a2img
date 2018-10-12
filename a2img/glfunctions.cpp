#include "glfunctions.h"

#include <QDebug>

#include <map>

a2img::GlFunctions::GlFunctions()
{

}


void a2img::GlFunctions::reportError(QString label, GLenum error)
{
	static std::map<GLenum, QString> errorMessages {
		{ GL_NO_ERROR, "GL_NO_ERROR" },
		{ GL_INVALID_ENUM, "GL_INVALID_ENUM" },
		{ GL_INVALID_VALUE, "GL_INVALID_VALUE" },
		{ GL_INVALID_OPERATION, "GL_INVALID_OPERATION" },
		{ GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" },
		{ GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY" }
	};

	QString message;
	if(errorMessages.find(error) != errorMessages.end()) {
		message = errorMessages[error];
	} else {
		message = "(unknown error)";
	}

	qDebug() << qPrintable(QString("[") + label + "]") << qPrintable(message);
}

void a2img::GlFunctions::testForError(QString label)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR || true) {
		reportError(label, error);
	}
}

/*
a2img::GraphicsVendor a2img::GLFunctions::getGraphicsVendor()
{
	return GraphicsVendor::intel;
}
*/

QString a2img::GlFunctions::getString(GLenum name)
{
	return reinterpret_cast<const char*>(glGetString(name));
}

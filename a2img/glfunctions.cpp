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

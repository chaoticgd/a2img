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

#ifndef GLFUNCTIONS_H
#define GLFUNCTIONS_H

#include <QString>
#include <QOpenGLFunctions_2_1>

// OpenGL error reporting mechanism.
// A2_GL_ENTER to be called when a segment of OpenGL code has begun executing.
// A2_GL_EXIT to be called when the segment has finished executing.
//#define A2_DEBUGMODE

#ifdef A2_DEBUGMODE
	#define A2_GL_ENTER(gl) (gl)->testForError(QString("Entering ") + __PRETTY_FUNCTION__);
	#define A2_GL_EXIT(gl) (gl)->testForError(QString("Exiting ") + __PRETTY_FUNCTION__);
#else
	#define A2_GL_ENTER(gl)
	#define A2_GL_EXIT(gl)
#endif

namespace a2img {
	/*enum class GraphicsVendor {
		intel = 0,
		nvidia = 1,
		amd = 2
	};*/

	class GlFunctions : public QOpenGLFunctions_2_1
	{
	public:
		GlFunctions();

		void reportError(QString label, GLenum error);
		void testForError(QString label);

		// GraphicsVendor getGraphicsVendor();

		template <typename... T_Buffers>
		void drawArrays(T_Buffers&... otherBuffers);

		template <typename T_ThisBuffer>
		void drawArrays(GLuint depth, T_ThisBuffer& thisBuffer);

		template <typename T_ThisBuffer, typename... T_OtherBuffers>
		void drawArrays(GLuint depth, T_ThisBuffer& thisBuffer, T_OtherBuffers&... otherBuffers);

		QString getString(GLenum name);
	};
}

template <typename... T_Buffers>
void a2img::GlFunctions::drawArrays(T_Buffers&... buffers)
{
	drawArrays(0, buffers...);
}

template <typename T_ThisBuffer>
void a2img::GlFunctions::drawArrays(GLuint depth, T_ThisBuffer& thisBuffer)
{
	thisBuffer.enable(depth);
	glDrawArrays(GL_TRIANGLES, 0, thisBuffer.numVertices);
	glDisableVertexAttribArray(depth);
}

template <typename T_ThisBuffer, typename... T_OtherBuffers>
void a2img::GlFunctions::drawArrays(GLuint depth, T_ThisBuffer& thisBuffer, T_OtherBuffers&... otherBuffers)
{
	thisBuffer.enable(depth);
	drawArrays(depth + 1, otherBuffers...);
	glDisableVertexAttribArray(depth);
}

#endif // GLFUNCTIONS_H

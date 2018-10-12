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

#ifndef GLBUFFER_H
#define GLBUFFER_H

#include <array>

#include "glfunctions.h"

namespace a2img {
	template <typename T_ComponentType, int T_numDimensions, int T_numVertices>
	class GlBuffer
	{
	public:
		GlBuffer(GlFunctions* gl, std::array<float, T_numDimensions * T_numVertices> data);
		~GlBuffer();

		void enable(GLuint n);

		int numVertices = T_numVertices;

	private:
		GlFunctions* gl_;
		GLuint id_;
	};

	using VertexBuffer = GlBuffer<float, 3, 6>;
	using UvBuffer = GlBuffer<float, 2, 6>;

	class DefaultVertexBuffer : public VertexBuffer
	{
	public:
		DefaultVertexBuffer(GlFunctions* gl);
	};

	class DefaultUvBuffer : public UvBuffer
	{
	public:
		DefaultUvBuffer(GlFunctions* gl);
	};
}

template <typename T_ComponentType, int T_numDimensions, int T_numVertices>
a2img::GlBuffer<T_ComponentType, T_numDimensions, T_numVertices>::GlBuffer(GlFunctions* gl, std::array<float, T_numDimensions * T_numVertices> data)
	: gl_(gl)
{
	gl_->glGenBuffers(1, &id_);
	gl_->glBindBuffer(GL_ARRAY_BUFFER, id_);
	gl_->glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T_ComponentType), static_cast<GLvoid*>(data.data()), GL_STATIC_DRAW);
}

template <typename T_ComponentType, int T_numDimensions, int T_numVertices>
a2img::GlBuffer<T_ComponentType, T_numDimensions, T_numVertices>::~GlBuffer()
{
	gl_->glDeleteBuffers(1, &id_);
}

template <typename T_ComponentType, int T_numDimensions, int T_numVertices>
void a2img::GlBuffer<T_ComponentType, T_numDimensions, T_numVertices>::enable(GLuint n)
{
	gl_->glEnableVertexAttribArray(n);
	gl_->glBindBuffer(GL_ARRAY_BUFFER, id_);
	gl_->glVertexAttribPointer(n, T_numDimensions, GL_FLOAT, GL_FALSE, 0, nullptr);
}

#endif // GLBUFFER_H

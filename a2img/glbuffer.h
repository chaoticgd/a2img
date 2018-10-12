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

#include "glbuffer.h"

#include "util/rectangle.h"

a2img::DefaultVertexBuffer::DefaultVertexBuffer(GlFunctions* gl)
    : VertexBuffer(gl, Rectangle<float>(-1, -1, 1, 1).vertexData()) {}

a2img::DefaultUvBuffer::DefaultUvBuffer(GlFunctions* gl)
    : UvBuffer(gl, Rectangle<float>(0, 0, 1, 1).uvData()) {}

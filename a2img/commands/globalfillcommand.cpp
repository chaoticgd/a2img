#include "globalfillcommand.h"

#include "application.h"
#include "util/rectangle.h"
#include "glbuffer.h"

a2img::GlobalFillCommand::GlobalFillCommand(Layer* layer, QColor colour, int tolerance)
	: FillCommand(layer, colour, tolerance)
{
	loadShaders();
}

a2img::Texture a2img::GlobalFillCommand::renderToTexture(GlFunctions* gl, const Texture& in, bool preview)
{
	QOpenGLFramebufferObject fbo(document()->size());
	fbo.bind();

	gl->glViewport(0, 0, document()->size().width(), document()->size().height());
	gl->glClearColor(0, 0, 0, 1);
	gl->glClear(GL_COLOR_BUFFER_BIT);
	shader_.bind();

	// Sampler

	GLint samplerLocation = shader_.uniformLocation("sampler");
	gl->glUniform1i(samplerLocation, 0);

	in.bind(gl, 0, TextureQuality::pixelated);

	// Resolution

	GLint resolutionUniform = shader_.uniformLocation("resolution");
	gl->glUniform2f(resolutionUniform, document()->size().width(), document()->size().height());

	// Starting position

	GLint startingPositionUniform = shader_.uniformLocation("startingPosition");
	gl->glUniform2f(startingPositionUniform, startingPoint().x(), startingPoint().y());

	// Tolerance

	GLint toleranceUniform = shader_.uniformLocation("tolerance");
	gl->glUniform1f(toleranceUniform, tolerance() / 255.0F);

	// Draw

	DefaultVertexBuffer vertexBuffer(gl);
	gl->drawArrays(vertexBuffer);

	Texture mask(app(), fbo.texture(), fbo.size());

	QColor renderColour = colour();
	if(preview) {
		renderColour.setAlpha(100);
	}
	return app()->maskManager().applyMask(gl, renderColour, in, mask, BlendMode::normal);
}

void a2img::GlobalFillCommand::loadShaders()
{
	app()->canvas()->draw([=](GlFunctions*) {
		shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/global_fill_vertex.glsl");
		shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/global_fill_fragment.glsl");
		shader_.bindAttributeLocation("position", 0);
		shader_.link();
	});
}


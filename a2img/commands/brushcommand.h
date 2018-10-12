#ifndef BRUSHCOMMAND_H
#define BRUSHCOMMAND_H

#include <vector>

#include <QPointF>
#include <QColor>
#include <QOpenGLShaderProgram>

#include "../glfunctions.h"
#include "texturestoragecommand.h"
#include "colour/colourmanager.h"
#include "maskmanager.h"

namespace a2img {
	class Application;

	class BrushCommand : public TextureStorageCommand
	{
	public:
		BrushCommand(Layer* layer, int diameter, QColor colour, BlendMode blendMode);

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;
		Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview);
		void populateInputPanel(QWidget* outer);

		void appendPoint(QPointF newPoint);

	protected:
		void drawPainter(QPainter* painter);

	private:
		std::vector<QPointF> points_;
		int diameter_;
		QColor colour_;
		BlendMode blendMode_;
	};
}

#endif // BRUSHCOMMAND_H

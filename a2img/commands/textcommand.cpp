#include "textcommand.h"

#include <QJsonObject>

#include "application.h"

a2img::TextCommand::TextCommand(Layer* layer)
	: TextureStorageCommand(layer)
{

}
QString a2img::TextCommand::type() const
{
	return "insertText";
}

void a2img::TextCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject json;
	json.insert("text", text_);
	metadata.insert("TextCommand", json);

	TextureStorageCommand::serialise(storage, metadata);
}

a2img::Texture a2img::TextCommand::renderToTexture(GlFunctions* gl, const Texture& in, bool preview)
{
	// Draw white-on-black mask.
	QImage maskImage(document()->size().width(), document()->size().height(), QImage::Format_RGB32);
	maskImage.fill(QColor(0, 0, 0, 0));
	QPainter maskPainter(&maskImage);
	maskPainter.setBrush(QColor(255, 255, 255));
	maskPainter.drawText(position_, text_);
	maskPainter.end();

	// Draw final image.
	const Texture mask(app(), gl, maskImage);

	return app()->maskManager().applyMask(gl, QColor(255, 0, 0), in, mask, BlendMode::normal);
}

void a2img::TextCommand::populateInputPanel(QWidget* outer)
{

}

QString a2img::TextCommand::text()
{
	return text_;
}

void a2img::TextCommand::setText(QString text)
{
	text_ = text;
	app()->canvas()->update(RedrawMode::fullRedraw);
}

QPoint a2img::TextCommand::position()
{
	return position_;
}

void a2img::TextCommand::setPosition(QPoint position)
{
	position_ = position;
	app()->canvas()->update(RedrawMode::fullRedraw);
}

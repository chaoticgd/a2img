#ifndef TEXTCOMMAND_H
#define TEXTCOMMAND_H

#include "texturestoragecommand.h"

namespace a2img {
	class TextCommand : public TextureStorageCommand
	{
	public:
		TextCommand(Layer* layer);

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;
		Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview);
		void populateInputPanel(QWidget* outer);

		QString text();
		void setText(QString text);

		QPoint position();
		void setPosition(QPoint position);

	private:
		QString text_;
		QPoint position_;
	};
}

#endif // TEXTCOMMAND_H

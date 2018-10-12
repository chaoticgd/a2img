#ifndef FILLCOMMAND_H
#define FILLCOMMAND_H

#include "texturestoragecommand.h"

namespace a2img {
	class FillCommand : public TextureStorageCommand
	{
	protected:
		FillCommand(Layer* layer, QColor colour, int tolerance);

	public:
		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

		void populateInputPanel(QWidget* outer);

		virtual void setStartingPoint(QPointF point);

	protected:
		QPoint startingPoint();
		QColor colour();
		int tolerance();

	private:
		QPoint startingPoint_;
		QColor colour_;
		int tolerance_;
	};
}

#endif // FILLCOMMAND_H

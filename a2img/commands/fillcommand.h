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

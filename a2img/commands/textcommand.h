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

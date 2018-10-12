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

#ifndef DOCUMENTWRITER_H
#define DOCUMENTWRITER_H

#include <map>
#include <array>
#include <QByteArray>
#include <QString>

#include <archive.h>
#include <archive_entry.h>

#include "glfunctions.h"
#include "texture.h"

namespace a2img {
	class Application;
	class Document;

	class DocumentWriter
	{
	public:
		DocumentWriter(GlFunctions* gl, const Document* document, QString path);
		~DocumentWriter();

		void addEntry(QString path, QByteArray data);
		void addTexture(const Texture *texture);
		void addJson(QString path, QJsonObject data);

	private:
		archive* data_;

		GlFunctions* gl_;
	};
}

#endif // DOCUMENTWRITER_H

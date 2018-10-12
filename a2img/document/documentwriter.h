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

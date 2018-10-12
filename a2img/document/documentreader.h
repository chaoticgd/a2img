#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H

#include <QString>

#include <archive.h>
#include <archive_entry.h>

#include "glfunctions.h"
#include "document.h"

namespace a2img {
	class DocumentReader
	{
	public:
		DocumentReader(GlFunctions* gl, Document* document, QString path);

	private:
		archive* data_;
	};
}

#endif // DOCUMENTREADER_H

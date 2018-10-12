#include "documentwriter.h"

#include <stdexcept>
#include <QFile>
#include <QBuffer>

#include "application.h"
#include "util/type.h"
#include "util/serialisation.h"
#include "commands/serialisablecommand.h"

a2img::DocumentWriter::DocumentWriter(GlFunctions* gl, const Document* document, QString path)
	: gl_(gl)
{
	std::string pathStr = path.toStdString();

	data_ = archive_write_new();
	archive_write_add_filter_gzip(data_);
	archive_write_set_format_pax_restricted(data_);
	archive_write_open_filename(data_, pathStr.c_str());

	// Store basic image information (in '/image.json').
	QJsonObject imageJson;
	imageJson.insert("applicationVersion", APPLICATION_VERSION.toString());
	imageJson.insert("fileFormatVersion", FILE_FORMAT_VERSION.toString());
	imageJson.insert("dimensions", serialisation::toJsonObject(document->size()));
	addJson("image.json", imageJson);

	// Store layer metadata (in '/layers.json').
	addJson("layers.json", document->rootLayer()->serialiseMetadata());

	// Store layer images (in '/images/').
	for(Layer* layer : document->layers()) {
		for(Texture* texture : layer->maps()) {
			addTexture(texture);
		}
	}

	// Store undo history (in '/history/' and '/history.json').
	QUndoStack* history = document->history_;
	for(int i = 0; i < history->count(); i++) {
		const SerialisableCommand* command =
			static_cast<const SerialisableCommand*>(history->command(i));

		QJsonObject commandMetadataJson;
		commandMetadataJson.insert("type", command->type());
		command->serialise(*this, commandMetadataJson);
		addJson("history/" + QString::number(i) + ".json", commandMetadataJson);
	}
}

a2img::DocumentWriter::~DocumentWriter()
{
	archive_write_close(data_);
	archive_write_free(data_);
}

void a2img::DocumentWriter::addEntry(QString path, QByteArray data)
{
	std::string pathStr = path.toLower().toStdString();

	archive_entry* entry = archive_entry_new();
	archive_entry_set_pathname(entry, pathStr.c_str());
	archive_entry_set_size(entry, data.size());
	archive_entry_set_filetype(entry, AE_IFREG);
	archive_entry_set_perm(entry, 0644);
	archive_write_header(data_, entry);
	archive_write_data(data_, data.data(), static_cast<size_t>(data.size()));
	archive_entry_free(entry);
}

void a2img::DocumentWriter::addTexture(const Texture* texture)
{
	QString fileName = "images/" + QString::number(reinterpret_cast<long>(texture), 16) + ".png";
	QImage image = texture->toQImage(gl_);
	QByteArray imageArray;
	QBuffer imageStream(&imageArray);
	image.save(&imageStream, "png");
	addEntry(fileName, imageArray);
}

void a2img::DocumentWriter::addJson(QString path, QJsonObject data)
{
	QJsonDocument layersJson(data);
	QByteArray layersMetadata = layersJson.toJson();
	addEntry(path, layersMetadata);
}

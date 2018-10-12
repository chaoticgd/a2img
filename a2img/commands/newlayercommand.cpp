#include "newlayercommand.h"

#include "application.h"

a2img::NewLayerCommand::NewLayerCommand(Layer* parent, int row)
	: LayerCommand(new Layer(parent->app(), parent->document(), "New Layer")),
	  newLayer_(layer()),
	  parentLayer_(parent)
{
	newLayer_->create(document()->size(), QColor(0, 0, 0, 0));

	LayerModel* model = document()->layerModel();
	if(row == -1) {
		row_ = model->rowCount(model->indexFromLayer(parent));
	} else {
		row_ = row;
	}
}

void a2img::NewLayerCommand::redo()
{
	LayerModel* model = document()->layerModel();
	model->addLayer(this, parentLayer_, newLayer_, row_);
}

void a2img::NewLayerCommand::undo()
{
	LayerModel* model = document()->layerModel();
	newLayer_ = model->removeLayer(this, layer());
}

QString a2img::NewLayerCommand::type() const
{
	return "newLayer";
}

void a2img::NewLayerCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject newLayerCommand;
	newLayerCommand.insert("row", row_);
	newLayerCommand.insert("parentLayerId", parentLayer_->id());
	if(newLayer_ != nullptr) {
		newLayerCommand.insert("newLayer", newLayer_->serialiseMetadata());

		for(Texture* map : newLayer_->maps()) {
			storage.addTexture(map);
		}
	}
	metadata.insert("new", newLayerCommand);

	LayerCommand::serialise(storage, metadata);
}

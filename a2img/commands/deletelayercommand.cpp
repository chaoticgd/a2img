#include "deletelayercommand.h"

#include "application.h"

a2img::DeleteLayerCommand::DeleteLayerCommand(Layer* layerArgument)
	: LayerCommand(layerArgument)
{
	parentLayer_ = layer()->parent();
	row_ = document()->selectedLayerIndex().row() + 1;
}

void a2img::DeleteLayerCommand::redo()
{
	LayerModel* model = document()->layerModel();
	ownedLayer_ = model->removeLayer(this, layer());
}

void a2img::DeleteLayerCommand::undo()
{
	LayerModel* model = document()->layerModel();
	model->addLayer(this, parentLayer_, ownedLayer_, row_);
}

QString a2img::DeleteLayerCommand::type() const
{
	return "deleteLayer";
}

void a2img::DeleteLayerCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject json;
	json.insert("parentLayerId", parentLayer_->id());
	json.insert("row", row_);
	if(ownedLayer_.get() != nullptr) {
		json.insert("deletedLayer", ownedLayer_->serialiseMetadata());

		for(Texture* map : ownedLayer_->maps()) {
			storage.addTexture(map);
		}
	}
	metadata.insert("delete", json);

	LayerCommand::serialise(storage, metadata);
}

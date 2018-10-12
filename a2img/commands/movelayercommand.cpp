#include "movelayercommand.h"

#include "application.h"

a2img::MoveLayerCommand::MoveLayerCommand(Layer* layer, bool isMovingUp)
	: LayerCommand(layer),
	  isMovingUp_(isMovingUp)
{}

void a2img::MoveLayerCommand::redo()
{
	if(isMovingUp_) {
		moveLayer(-1);
	} else {
		moveLayer(1);
	}
}

void a2img::MoveLayerCommand::undo()
{
	if(isMovingUp_) {
		moveLayer(1);
	} else {
		moveLayer(-1);
	}
}

QString a2img::MoveLayerCommand::type() const
{
	return "moveLayer";
}

void a2img::MoveLayerCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject json;
	json.insert("direction", isMovingUp_ ? "up" : "down");
	metadata.insert("move", json);
}

void a2img::MoveLayerCommand::moveLayer(int offset)
{
	LayerModel* model = document()->layerModel();
	int row = model->indexFromLayer(layer()).row();
	std::unique_ptr<Layer> uniqueLayer = model->removeLayer(this, layer());
	model->addLayer(this, layer()->parent(), uniqueLayer, row + offset);

	QModelIndex newIndex = model->indexFromLayer(layer());
	app()->documentManager().setCurrentLayerIndex(newIndex);
}

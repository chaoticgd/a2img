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

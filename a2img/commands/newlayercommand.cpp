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

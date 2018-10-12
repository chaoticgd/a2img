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

#include "texturestoragecommand.h"

#include "application.h"
#include "layer.h"
#include "../util/type.h"

a2img::TextureStorageCommand::TextureStorageCommand(Layer* layer)
	: LayerCommand(layer),
	  isSelecting_(false),
	  beforeTexture_(layer->app())
{
}

void a2img::TextureStorageCommand::redo()
{
	app()->canvas()->draw([=](GlFunctions* gl) {
		layer()->replaceTexture(this, [=, &gl](Texture& in) {
			beforeTexture_= std::move(in);

			Texture output(renderToTexture(gl, beforeTexture_, false));

			// Limit image modifications to the selected region.
			if(isSelecting_) {
				output = app()->maskManager().applyMask(gl, output, beforeTexture_, *selectionMask_.get());
			}

			return output;
		});
	});
}

void a2img::TextureStorageCommand::undo()
{
	app()->canvas()->draw([=](GlFunctions*) {
		layer()->replaceTexture(this, [=](Texture&) {
			return std::move(beforeTexture_);
		});
	});
}

void a2img::TextureStorageCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject textureStorageCommand;
	textureStorageCommand.insert("isSelecting", isSelecting_);
	metadata.insert("texture", textureStorageCommand);

	LayerCommand::serialise(storage, metadata);
}

void a2img::TextureStorageCommand::onApplied()
{
	Document* doc = app()->documentManager().currentDocument();
	isSelecting_ = doc->isRegionSelected();
	selectionMask_ = doc->shareSelectionMask();
}

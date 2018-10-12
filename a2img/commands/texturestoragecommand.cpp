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

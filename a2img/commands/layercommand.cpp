#include "layercommand.h"

#include "application.h"

a2img::LayerCommand::LayerCommand(Layer* layer)
	: layer_(layer)
{}

void a2img::LayerCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject layerCommand;
	layerCommand.insert("layerId", layer_->id());
	metadata.insert("layer", layerCommand);
}

a2img::Application* a2img::LayerCommand::app()
{
	return layer_->app();
}

a2img::Document* a2img::LayerCommand::document()
{
	return layer_->document();
}

a2img::Layer* a2img::LayerCommand::layer()
{
	return layer_;
}

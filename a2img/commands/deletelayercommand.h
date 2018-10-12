#ifndef DELETELAYERCOMMAND_H
#define DELETELAYERCOMMAND_H

#include <memory>

#include <QModelIndex>

#include "../layer.h"
#include "layercommand.h"

namespace a2img {
	class Application;

	class DeleteLayerCommand : public LayerCommand
	{
	public:
		DeleteLayerCommand(Layer* layerArgument);

		void redo();
		void undo();

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

	private:
		Application* app_;
		Layer* parentLayer_;
		int row_;
		std::unique_ptr<Layer> ownedLayer_;
	};
}

#endif // NEWLAYERCOMMAND_H

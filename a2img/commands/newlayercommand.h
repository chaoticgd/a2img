#ifndef NEWLAYERCOMMAND_H
#define NEWLAYERCOMMAND_H

#include <memory>

#include <QModelIndex>

#include "layercommand.h"
#include "../layer.h"

namespace a2img {
	class Application;

	class NewLayerCommand : public LayerCommand
	{
	public:
		NewLayerCommand(Layer* parent, int row);

		void redo();
		void undo();

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

	private:
		std::unique_ptr<Layer> newLayer_;
		Layer* parentLayer_;
		int row_;
	};
}

#endif // NEWLAYERCOMMAND_H

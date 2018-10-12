#ifndef MOVELAYERCOMMAND_H
#define MOVELAYERCOMMAND_H

#include <memory>

#include <QModelIndex>

#include "../layer.h"
#include "layercommand.h"

namespace a2img {
	class Application;

	class MoveLayerCommand : public LayerCommand
	{
	public:
		MoveLayerCommand(Layer* layer, bool isMovingUp);

		void redo();
		void undo();

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

		void moveLayer(int offset);

	private:
		bool isMovingUp_;
	};
}

#endif // MOVELAYERCOMMAND_H

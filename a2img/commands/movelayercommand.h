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

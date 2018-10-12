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

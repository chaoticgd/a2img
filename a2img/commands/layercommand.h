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

#ifndef LAYERCOMMAND_H
#define LAYERCOMMAND_H

#include "serialisablecommand.h"

namespace a2img {
	class Application;
	class Document;
	class Layer;

	typedef qint64 LayerId;

	// A command that affects a specific layer.
	class LayerCommand : public SerialisableCommand
	{
	protected:
		LayerCommand(Layer* layer);

		virtual void serialise(DocumentWriter& storage, QJsonObject& metadata) const = 0;

		Application* app();

	public:
		Document* document();
		Layer* layer();

	private:
		Application* app_;
		Document* document_;
		Layer* layer_;
	};
}

#endif // LAYERCOMMAND_H

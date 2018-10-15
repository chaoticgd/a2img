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

#ifndef LAYERMODEL_H
#define LAYERMODEL_H

#include <map>

#include <QAbstractItemView>

#include "layer.h"

namespace a2img {
	class Application;

	class LayerModel : public QAbstractItemModel
	{
	public:
		LayerModel(Application* app, Document* document);
		~LayerModel();

		QVariant data(const QModelIndex& index, int role) const override;
		Qt::ItemFlags flags(const QModelIndex&index) const override;
		QVariant headerData(int, Qt::Orientation, int) const override;
		QModelIndex index(int row, int column,
							  const QModelIndex& parent = QModelIndex()) const override;
		QModelIndex parent(const QModelIndex& index) const override;
		int rowCount(const QModelIndex&parent = QModelIndex()) const override;
		int columnCount(const QModelIndex& = QModelIndex()) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role);

		void addLayer(QUndoCommand*, Layer* parent, std::unique_ptr<Layer>& layer, int row);
		std::unique_ptr<Layer> removeLayer(QUndoCommand*, Layer* layer);

		QModelIndex indexFromLayer(Layer* targetLayer, QModelIndex parent = QModelIndex());
		QModelIndex indexFromLayerId(LayerId id);

	private:
		Application* app_;
		Document* document_;
	};
}

#endif // LAYERMODEL_H

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

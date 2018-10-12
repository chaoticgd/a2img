#include "layermodel.h"

#include <QPalette>

#include "application.h"

a2img::LayerModel::LayerModel(Application* app, Document* document)
	: app_(app),
	  document_(document)
{
}

a2img::LayerModel::~LayerModel()
{

}

QVariant a2img::LayerModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	Layer* layer = static_cast<Layer*>(index.internalPointer());
	if(layer == nullptr) {
		return QVariant();
	}


	QVariant result;

	switch(role) {
		case Qt::DisplayRole:
			result = layer->displayName();
			break;
		case Qt::CheckStateRole:
			result = layer->isVisible() ? Qt::Checked : Qt::Unchecked;
			break;

		// Show selection colours.
		case Qt::ForegroundRole:
		case Qt::BackgroundRole: {
			QPalette::ColorRole colour;
			bool isSelected = layer == app_->documentManager().currentDocument()->selectedLayer();
			if(role == Qt::ForegroundRole && isSelected) {
				colour = QPalette::HighlightedText;
			} else if(role == Qt::BackgroundRole) {
				if(isSelected) {
					colour = QPalette::Highlight;
				} else {
					colour = QPalette::Base;
				}
			} else {
				break;
			}
			result = app_->window()->palette().color(colour);
			break;

		}
		// Display the thumbnail.
		case Qt::DecorationRole:
			result = layer->thumbnail();
	}

	return result;
}

Qt::ItemFlags a2img::LayerModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QVariant a2img::LayerModel::headerData(int, Qt::Orientation, int) const
{
	return "";
}

QModelIndex a2img::LayerModel::index(int row, int column, const QModelIndex& parent) const
{
	if(!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	Layer* parentLayer;
	if(!parent.isValid()) {
		parentLayer = app_->documentManager().currentDocument()->rootLayer();
	} else {
		parentLayer = static_cast<Layer*>(parent.internalPointer());
	}

	Layer* childLayer = parentLayer->children()[static_cast<size_t>(row)];
	if(childLayer == nullptr) {
		return QModelIndex();
	}

	return createIndex(row, column, childLayer);
}

QModelIndex a2img::LayerModel::parent(const QModelIndex &index) const
{
	if(!index.isValid()) {
		return QModelIndex();
	}

	Layer* layer = static_cast<Layer*>(index.internalPointer());
	Layer* parent = layer->parent();

	if(parent == nullptr) {
		return QModelIndex();
	}

	Layer* superParent = parent->parent();

	if(superParent == nullptr) {
		return QModelIndex();
	}

	int row = 0;
	for(size_t i = 0; i < superParent->children().size(); i++) {
		if(superParent->children()[i] == parent) {
			row = static_cast<int>(i);
		}
	}
	return createIndex(row, 0, parent);
}

int a2img::LayerModel::rowCount(const QModelIndex& parent) const
{
	if(parent.column() > 0) {
		return 0;
	}

	Layer* parentLayer;

	if(parent.isValid()) {
		parentLayer = static_cast<Layer*>(parent.internalPointer());
	} else {
		parentLayer = app_->documentManager().currentDocument()->rootLayer();
	}

	return static_cast<int>(parentLayer->children().size());
}

int a2img::LayerModel::columnCount(const QModelIndex&) const
{
	return 1;
}

bool a2img::LayerModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	Layer* layer = static_cast<Layer*>(index.internalPointer());
	if(role == Qt::CheckStateRole) {
		layer->setVisible(value.toBool());
	} else {
		layer->setDisplayName(value.toString());
	}
	return true;
}

void a2img::LayerModel::addLayer(QUndoCommand*, Layer* parent, std::unique_ptr<Layer>& layer, int row)
{
	QModelIndex parentIndex = indexFromLayer(parent);

	if(parent == nullptr) {
		parent = document_->rootLayer();
	}
	int position = static_cast<int>(parent->children().size());
	beginInsertRows(parentIndex, position, position);
	parent->insertChild(layer, static_cast<size_t>(row));
	endInsertRows();
}

std::unique_ptr<a2img::Layer> a2img::LayerModel::removeLayer(QUndoCommand*, Layer* layer)
{
	QModelIndex layerIndex = indexFromLayer(layer);

	Layer* parentLayer = layer->parent();
	if(parentLayer == nullptr) {
		parentLayer = app_->documentManager().currentDocument()->rootLayer();
	}
	auto children = parentLayer->children();
	int row = static_cast<int>(
		std::distance(children.begin(), std::find(children.begin(), children.end(), layer))
	);
	beginRemoveRows(layerIndex.parent(), row, row);
	std::unique_ptr<Layer> ownedLayer = parentLayer->removeChild(layer);
	endRemoveRows();
	return ownedLayer;
}

QModelIndex a2img::LayerModel::indexFromLayer(Layer* targetLayer, QModelIndex parent)
{
	if(static_cast<Layer*>(parent.internalPointer()) == targetLayer) {
		return parent;
	}

	if(hasChildren(parent)) {
		for(int i = 0; i < rowCount(parent); i++) {
			QModelIndex childIndex = indexFromLayer(targetLayer, index(i, 0, parent));
			if(childIndex.isValid()) {
				return childIndex;
			}
		}
	}

	return QModelIndex();
}

QModelIndex a2img::LayerModel::indexFromLayerId(LayerId id)
{
	return indexFromLayer(document_->layerFromId(id));
}

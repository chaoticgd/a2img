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

#include "document.h"

#include <stdexcept>

#include <QImage>
#include <QUndoStack>
#include <QFile>

#include "application.h"
#include "document/documentwriter.h"

a2img::Document::Document(Application* app, QSize size, QColor backgroundColour)
	: app_(app),
	  fileName_(""),
	  containingDirectory_(""),
	  size_(size),
	  format_(ImageFormat::png),
	  rootLayer_(new Layer(app, this, "<root>")),
	  layerModel_(new LayerModel(app, this)),
	  nextLayerHandle_(1),
	  history_(new QUndoStack(app->window()))
{
	create();
	Layer* defaultLayer = rootLayer_->createChild<Layer>(app, this, "Default Layer");
	defaultLayer->create(size, backgroundColour);
}

a2img::Document::Document(Application* app, QString filePath)
	: app_(app),
	  fileName_(filePath.mid(filePath.lastIndexOf("/") + 1)),
	  containingDirectory_(filePath.mid(0, filePath.lastIndexOf("/"))),
	  rootLayer_(new Layer(app, this, "<root>")),
	  layerModel_(new LayerModel(app, this)),
	  nextLayerHandle_(1), // TODO: FIX THIS!!!!!!!!!!!!!!!
	  history_(new QUndoStack(app->window()))
{
	create();

	QImage image(filePath);

	size_ = image.size();

	// Determine the image format from the extenion.
	QString fileExtension = fileName_.mid(fileName_.lastIndexOf('.') + 1).toLower();
	if(fileExtension == "jpg" || fileExtension == "jpeg") {
		format_ = ImageFormat::jpg;
	} else if(fileExtension == "bmp") {
		format_ = ImageFormat::bmp;
	} else {
		format_ = ImageFormat::png;
	}
	Layer* defaultLayer = rootLayer_->createChild<Layer>(app, this, "Default Layer");
	defaultLayer->create(image);
}

void a2img::Document::create()
{
	rootLayer_->create(size_, QColor(0, 0, 0, 0));
	history_->setUndoLimit(50);

	QObject::connect(
		layerModel_.get(), &QAbstractItemModel::dataChanged,
		[=](const QModelIndex&, const QModelIndex&, const QVector<int>&) {
			app_->documentManager().updateUserInterface();
		}
	);
}

/*
	Getters/Setters
*/

QString a2img::Document::fileName() const
{
	return fileName_;
}

QString a2img::Document::path() const
{
	return containingDirectory_ + "/" + fileName_;
}

QSize a2img::Document::size() const
{
	return size_;
}

a2img::ImageFormat a2img::Document::format() const
{
	return format_;
}

double a2img::Document::getUserZoom() const
{
	return app_->canvas()->getZoomPercentage() / 100;
}

/*
	Layers
*/

std::vector<a2img::Layer*> a2img::Document::layers() const
{
	std::vector<a2img::Layer*> current { rootLayer_.get() }, output;

	while(current.size() > 0) {
		a2img::Layer* layer = *(current.end() - 1);
		current.pop_back();

		for(Layer* child : layer->children()) {
			current.push_back(child);
		}
		if(layer->parent() != nullptr) { // Exclude the root.
			output.push_back(layer);
		}
	}

	return output;
}

bool a2img::Document::isSelectedLayerValid() const
{
	return selectedLayerIndex().isValid();
}

QModelIndex a2img::Document::selectedLayerIndex() const
{
	QModelIndex result = app_->documentManager().currentLayerIndex();

	// If there is no valid selection, assume the first child of the root layer is selected.
	auto rootLayerChildren = rootLayer_->children();
	if(!result.isValid() && rootLayerChildren.size() >= 1) {
		result = layerModel_->indexFromLayer(rootLayerChildren[0]);
	}

	return result;
}

a2img::Layer* a2img::Document::selectedLayer() const
{
	QModelIndex index = selectedLayerIndex();

	if(!index.isValid()) {
		throw std::runtime_error("The selected layer is invalid.");
	}

	return static_cast<Layer*>(index.internalPointer());
}

a2img::Layer* a2img::Document::rootLayer() const
{
	return rootLayer_.get();
}

a2img::LayerModel* a2img::Document::layerModel()
{
	return layerModel_.get();
}

a2img::LayerId a2img::Document::idFromLayer(Layer* layer) const
{
	for(auto pair : layerHandles_) {
		if(pair.second == layer) {
			return pair.first;
		}
	}
	return Layer::invalidId;
}

a2img::Layer* a2img::Document::layerFromId(LayerId id)
{
	if(layerHandles_.find(id) == layerHandles_.end()) {
		return nullptr;
	}
	return layerHandles_.at(id);
}

void a2img::Document::registerLayer(Layer* layer)
{
	layerHandles_.emplace(nextLayerHandle_++, layer);
}

/*
	Selection
*/

const a2img::Texture& a2img::Document::selectionMask() const
{
	return *selectionMask_.get();
}

std::shared_ptr<a2img::Texture> a2img::Document::shareSelectionMask() const
{
	return selectionMask_;
}

void a2img::Document::setSelectedRegion(Rectangle<int> rectangle)
{
	if(rectangle.size().x == 0 || rectangle.size().y == 0) {
		selectionMask_.reset();
		return;
	}

	app_->canvas()->draw([=](GlFunctions* gl) {
		selectionMask_.reset(new Texture(app_->maskManager().createMask(gl, Vec2<int>(size()), rectangle)));
	});
}

bool a2img::Document::isRegionSelected() const
{
	return selectionMask_.get() != nullptr;
}

void a2img::Document::save(QString filePath, ImageFormat format) const
{
	app_->canvas()->draw([=](GlFunctions* gl) {
		QOpenGLFramebufferObject fbo(size());
		rootLayer_->draw(gl, fbo, false);

		std::string formatStr;
		switch(format) {
			case a2img::ImageFormat::a2: {
				app_->canvas()->draw([=](GlFunctions* gl) {
					DocumentWriter writer(gl, this, filePath);
				});
				return; // Don't save the file twice.
			}
			case a2img::ImageFormat::png: {
				formatStr = "png";
				break;
			}
			case a2img::ImageFormat::jpg: {
				formatStr = "jpg";
				break;
			}
			case a2img::ImageFormat::bmp: {
				formatStr = "bmp";
				break;
			}
		}

		QImage outputImage = fbo.toImage();
		outputImage.save(filePath, formatStr.c_str());
	});
}

void a2img::Document::undo()
{
	history_->undo();
}

void a2img::Document::redo()
{
	history_->redo();
}

void a2img::Document::pushUndoCommand(QUndoCommand* command)
{
	TextureStorageCommand* tsc = dynamic_cast<TextureStorageCommand*>(command);
	if(tsc != nullptr) {
		tsc->onApplied();
	}
	history_->push(command);
}

/*
	Cooridinate Space Mappings
*/

QPointF a2img::Document::mapSceneToDocument(QPointF qtPosition)
{
	float zoom = app_->canvas()->getZoomPercentage() / 100;
	return {
		qtPosition.x() / zoom,
		size().height() - qtPosition.y() / zoom
	};
}

QPointF a2img::Document::mapDocumentToScene(QPointF documentPos)
{
	float zoom = app_->canvas()->getZoomPercentage() / 100;
	return {
		documentPos.x() * zoom,
		(size().height() - documentPos.y()) * zoom
	};
}

QPointF a2img::Document::mapDocumentToScene(QPoint documentPos)
{
	return mapDocumentToScene(QPointF(
		documentPos.x(),
		documentPos.y()
	));
}

QRectF a2img::Document::mapDocumentToScene(QRectF documentRect)
{
	QPointF firstPoint = mapDocumentToScene(documentRect.bottomLeft());
	QPointF secondPoint = mapDocumentToScene(documentRect.topRight());
	return {
		firstPoint.x(),
		firstPoint.y(),
		secondPoint.x() - firstPoint.x(),
		secondPoint.y() - firstPoint.y()
	};
}

void a2img::Document::setSize(ImageResizeCommand* command, QSize newSize)
{
	size_ = newSize;
}

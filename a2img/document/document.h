#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <memory>

#include <QSize>
#include <QColor>
#include <QUndoStack>
#include <QJsonDocument>

#include "layer.h"
#include "canvas.h"
#include "layermodel.h"
#include "util/rectangle.h"

namespace a2img {
	class Application;
	class ImageResizeCommand;

    enum class ImageFormat {
		a2,
        png,
        jpg,
        bmp
    };

    class Document
	{
		friend class DocumentWriter;
    public:
        // Create a new document.
		Document(Application* app, QSize size, QColor backgroundColour);

        // Load a document from disk.
		Document(Application* app, QString filePath);

	private:
		void create();

	public:

		/*
			Getters/Setters
		*/

        // Returns the file name of the document.
        // If unsaved, returns an empty string.
		QString fileName() const;

		// Returns the path of the file.
		QString path() const;

        // Returns the documents resolution in pixels.
		QSize size() const;

		// Get the current image format.
		ImageFormat format() const;

		// The users' view position and zoom level.
		double getUserZoom() const;

		/*
			Layers
		*/

		// Returns all layers recurisvely, excluding the root layer.
		std::vector<Layer*> layers() const;

		// Returns true if selectedLayer() and selectedLayerIndex() will return valid result.
		// Returns false if there are no layers in the document (besides the root).
		bool isSelectedLayerValid() const;

		// Returns a temporary QModelIndex associated with the selected layer.
		// This QModelIndex will be invalid if there are no layers in the document.
		QModelIndex selectedLayerIndex() const;

		// Returns a pointer to the selected layer.
		// If the associated QModelIndex is invalid, this function will throw a runtime_error.
		Layer* selectedLayer() const;

		// Returns the root layer.
		Layer* rootLayer() const;

		LayerModel* layerModel();

		LayerId idFromLayer(Layer* layer) const;
		Layer* layerFromId(LayerId id);

		// Called in Layer::create.
		void registerLayer(Layer* layer);

		/*
			Selection
		*/

		const Texture& selectionMask() const;

		std::shared_ptr<Texture> shareSelectionMask() const;

		// Sets a bounding box around the selected region.
		void setSelectedRegion(Rectangle<int> rectangle);

		// Checks if the size of the selected region is (0, 0).
		bool isRegionSelected() const;

		// Writes the document to disk.
		void save(QString filePath, ImageFormat format) const;

		void undo();
		void redo();
		void pushUndoCommand(QUndoCommand* command);

		/*
			Cooridinate Space Mappings
		*/

		QPointF mapSceneToDocument(QPointF qtPos);
		QPointF mapDocumentToScene(QPointF documentPos);
		QPointF mapDocumentToScene(QPoint documentPos);
		QRectF mapDocumentToScene(QRectF documentRect);

		template <typename T>
		Vec2<T> mapSceneToDocument(QPointF scenePos) const;

		template <typename T>
		Rectangle<T> mapSceneToDocument(QRectF scenePos) const;

		template <typename T>
		QPointF mapDocumentToScene(Vec2<T> documentPos) const;

		template <typename T>
		QRectF mapDocumentToScene(Rectangle<T> documentRect) const;

		// Can only be called from within an ImageResizeCommand.
		void setSize(ImageResizeCommand* command, QSize newSize);

	private:
		Canvas* canvas();

		Application* app_;

		QString fileName_;
		QString containingDirectory_;
		QSize size_;
		ImageFormat format_;

		ViewportData viewportData_;

		std::unique_ptr<Layer> rootLayer_;
		std::unique_ptr<LayerModel> layerModel_;
		std::map<LayerId, Layer*> layerHandles_;
		LayerId nextLayerHandle_;

		std::shared_ptr<Texture> selectionMask_;

		QUndoStack* history_;
	};
}

/*
	Cooridinate Space Mappings
*/

template <typename T>
a2img::Vec2<T> a2img::Document::mapSceneToDocument(QPointF scenePos) const
{
	double zoom = getUserZoom();
	return {
		static_cast<T>(scenePos.x() / zoom),
		static_cast<T>(size().height() - scenePos.y() / zoom)
	};
}

template <typename T>
a2img::Rectangle<T> a2img::Document::mapSceneToDocument(QRectF scenePos) const
{
	Vec2<int> firstPoint(mapSceneToDocument<T>(scenePos.bottomLeft()));
	Vec2<int> secondPoint(mapSceneToDocument<T>(scenePos.topRight()));
	return { firstPoint, secondPoint };
}

template <typename T>
QPointF a2img::Document::mapDocumentToScene(Vec2<T> documentPos) const
{
	double zoom = getUserZoom();
	return {
		documentPos.x * zoom,
		(size().height() - documentPos.y) * zoom
	};
}

template <typename T>
QRectF a2img::Document::mapDocumentToScene(Rectangle<T> documentRect) const
{
	Vec2<T> firstPoint = mapDocumentToScene(documentRect.bottomLeft());
	Vec2<T> secondPoint = mapDocumentToScene(documentRect.topRight());
	return { firstPoint.toQPointF(), secondPoint.toQPointF() };
}


#endif // DOCUMENT_H

#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <vector>
#include <QTreeView>
#include <QPushButton>

#include "document/document.h"

namespace a2img {
	class Application;

	class DocumentManager
	{
	public:
		DocumentManager(Application* app);

		// Returns true if there are one or more documents open.
		bool isDocumentOpen();

		// Returns a pointer to the active document.
		Document* currentDocument();

		// Open a document, and set it as the active document.
		void openDocument(QString filePath);

		// Creates a new document, given a size and background colour.
		void createDocument(QSize size, QColor backgroundColour);

		void setCurrentDocument(int index);

		void closeDocument(int index);

		std::vector<Document*> documents();

		QModelIndex currentLayerIndex();
		void setCurrentLayerIndex(QModelIndex index);

		void updateUserInterface();

		void loadLayersPanel();

		// Preview commands apply to the current layer temporarily.
		// For example, a brush stroke while the user is still drawing.
		TextureStorageCommand* getPreviewCommand();
		void setPreviewCommand(TextureStorageCommand* previewCommand);
		void clearPreviewCommand();

	private:
		void updateDocumentSelectionTabBar();
		void updateLayerButtonDisabledStates();

		Application* app_;

		std::vector<std::unique_ptr<Document>> documents_;
		std::vector<std::unique_ptr<Document>>::iterator currentDocument_;

		QTreeView* layersView_;

		QPushButton* newLayerButton;
		QPushButton* deleteLayerButton;
		QPushButton* moveLayerUpButton;
		QPushButton* moveLayerDownButton;

		TextureStorageCommand* previewCommand_;
	};
}

#endif // DOCUMENTMANAGER_H

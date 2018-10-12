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

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

#include "documentmanager.h"

#include <QVBoxLayout>
#include <QDockWidget>
#include <QMenu>
#include "application.h"
#include "commands/newlayercommand.h"
#include "commands/deletelayercommand.h"
#include "commands/movelayercommand.h"

a2img::DocumentManager::DocumentManager(Application* app)
	: app_(app),
	  currentDocument_(documents_.end()),
	  previewCommand_(nullptr)
{}

bool a2img::DocumentManager::isDocumentOpen()
{
	return currentDocument_ != documents_.end();
}

a2img::Document* a2img::DocumentManager::currentDocument()
{
	return currentDocument_->get();
}

void a2img::DocumentManager::openDocument(QString filePath)
{
	documents_.emplace_back(new Document(app_, filePath));
	currentDocument_ = documents_.end() - 1;
	updateUserInterface();
}

void a2img::DocumentManager::createDocument(QSize size, QColor backgroundColour)
{
	documents_.emplace_back(new Document(app_, size, backgroundColour));
	currentDocument_ = documents_.end() - 1;
	updateUserInterface();
}

void a2img::DocumentManager::setCurrentDocument(int index)
{
	currentDocument_ = documents_.begin() + index;
	updateUserInterface();
}

void a2img::DocumentManager::closeDocument(int index)
{
	auto iter = documents_.begin() + index;
	documents_.erase(iter);
	currentDocument_ = documents_.begin();
	updateUserInterface();
}

std::vector<a2img::Document*> a2img::DocumentManager::documents()
{
	std::vector<a2img::Document*> result(documents_.size());
	std::transform(
		documents_.begin(), documents_.end(), result.begin(),
		[](std::unique_ptr<Document>& document) { return document.get(); }
	);
	return result;
}

QModelIndex a2img::DocumentManager::currentLayerIndex()
{
	return layersView_->selectionModel()->currentIndex();
}

void a2img::DocumentManager::setCurrentLayerIndex(QModelIndex index)
{
	layersView_->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void a2img::DocumentManager::updateUserInterface()
{
	MainWindow* window = app_->window();

	if(isDocumentOpen()) {
		QString title = currentDocument()->fileName();
		if(title == "") {
			window->setWindowTitle("Untitled");
		} else {
			window->setWindowTitle(title);
		}
		layersView_->setModel(currentDocument()->layerModel());
		layersView_->setDisabled(false);

		// When a different layer is selected, update the UI.
		QObject::connect(
			layersView_->selectionModel(), &QItemSelectionModel::currentRowChanged,
			[=](const QModelIndex&, const QModelIndex&) {
				app_->effectManager().updatePanel();
				updateLayerButtonDisabledStates();
			}
		);

	} else {
		window->setWindowTitle("No Document");
		layersView_->setDisabled(true);
	}

	updateDocumentSelectionTabBar();
	app_->effectManager().updatePanel();
	updateLayerButtonDisabledStates();
	app_->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::DocumentManager::loadLayersPanel()
{
	QDockWidget* layersDockWidget = new QDockWidget("Layers");
	app_->window()->addDockWidget(Qt::LeftDockWidgetArea, layersDockWidget);

	QWidget* container = new QWidget;
	QVBoxLayout* layout = new QVBoxLayout;
	container->setLayout(layout);
	layersDockWidget->setWidget(container);

	layersView_ = new QTreeView;
	layersView_->setDisabled(true);
	layersView_->setColumnWidth(0, 32);
	layout->addWidget(layersView_);

	QWidget* buttonBox = new QWidget;
	QHBoxLayout* buttonBoxLayout = new QHBoxLayout;
	buttonBox->setLayout(buttonBoxLayout);
	layout->addWidget(buttonBox);

	newLayerButton = new QPushButton("+");
	newLayerButton->setMinimumWidth(32);
	buttonBoxLayout->addWidget(newLayerButton);

	// Create a new layer.
	QObject::connect(newLayerButton, &QPushButton::clicked, [=]() {
		QMenu menu(app_->window());
		menu.addAction("Add Sibling");
		menu.addAction("Add Child");

		QObject::connect(&menu, &QMenu::triggered, [=](QAction* action) {
			QModelIndex parent = currentDocument()->selectedLayerIndex();
			Layer* parentLayer = static_cast<Layer*>(parent.internalPointer());

			// If there is no valid selection, assume the root layer is the parent.
			if(parentLayer == nullptr) {
				parentLayer = currentDocument()->rootLayer();
			}

			int row;
			if(action->text() == "Add Sibling") {
				row = parent.row() + 1;
				parentLayer = parentLayer->parent();

			} else {
				row = -1;
			}

			NewLayerCommand* command = new NewLayerCommand(parentLayer, row);
			app_->documentManager().currentDocument()->pushUndoCommand(command);

			updateUserInterface();
		});

		menu.exec(app_->window()->cursor().pos());
	});

	// Delete the selected layer.
	deleteLayerButton = new QPushButton("-");
	deleteLayerButton->setMinimumWidth(32);
	buttonBoxLayout->addWidget(deleteLayerButton);

	QObject::connect(deleteLayerButton, &QPushButton::clicked, [=]() {
		Layer* selectedLayer = currentDocument()->selectedLayer();
		DeleteLayerCommand* command = new DeleteLayerCommand(selectedLayer);
		app_->documentManager().currentDocument()->pushUndoCommand(command);
		updateUserInterface();
	});

	// Move the selected layer.
	moveLayerUpButton = new QPushButton("\u2191");
	moveLayerUpButton->setMinimumWidth(32);
	buttonBoxLayout->addWidget(moveLayerUpButton);

	QObject::connect(moveLayerUpButton, &QPushButton::clicked, [=]() {
		Layer* selectedLayer = currentDocument()->selectedLayer();
		MoveLayerCommand* command = new MoveLayerCommand(selectedLayer, true);
		app_->documentManager().currentDocument()->pushUndoCommand(command);
		updateUserInterface();
	});

	moveLayerDownButton = new QPushButton("\u2193");
	moveLayerDownButton->setMinimumWidth(32);
	buttonBoxLayout->addWidget(moveLayerDownButton);

	QObject::connect(moveLayerDownButton, &QPushButton::clicked, [=]() {
		Layer* selectedLayer = currentDocument()->selectedLayer();
		MoveLayerCommand* command = new MoveLayerCommand(selectedLayer, false);
		app_->documentManager().currentDocument()->pushUndoCommand(command);
		updateUserInterface();
	});


	updateLayerButtonDisabledStates();
}

void a2img::DocumentManager::updateDocumentSelectionTabBar()
{
	QTabBar* documentTabBar = app_->window()->documentTabBar();

	documentTabBar->blockSignals(true);
	// Remove old tabs.
	while(documentTabBar->count() > 0) {
		documentTabBar->removeTab(0);
	}

	// Re-create all tabs.
	auto documentList = documents();
	for(size_t i = 0; i < documentList.size(); i++) {
		QString name = documentList[i]->fileName();
		if(name == "") {
			name = "Untitled";
		}
		documentTabBar->addTab(name);

		if(documentList[i] == app_->documentManager().currentDocument()) {
			documentTabBar->setCurrentIndex(static_cast<int>(i));
		}
	}
	documentTabBar->blockSignals(false);
}

void a2img::DocumentManager::updateLayerButtonDisabledStates()
{
	if(!isDocumentOpen()) {
		newLayerButton->setDisabled(true);
		deleteLayerButton->setDisabled(true);
		moveLayerUpButton->setDisabled(true);
		moveLayerDownButton->setDisabled(true);
		return;
	}

	QModelIndex index = currentLayerIndex();

	newLayerButton->setDisabled(false);

	auto rootChildren = currentDocument()->rootLayer()->children();
	deleteLayerButton->setDisabled(rootChildren.size() < 1);
	moveLayerUpButton->setDisabled(index.row() < 1);

	int lastRow = layersView_->model()->rowCount(index.parent()) - 1;
	moveLayerDownButton->setDisabled(index.row() == -1 || index.row() >= lastRow);
}

a2img::TextureStorageCommand* a2img::DocumentManager::getPreviewCommand()
{
	return previewCommand_;
}

void a2img::DocumentManager::setPreviewCommand(TextureStorageCommand* previewCommand)
{
	previewCommand_ = previewCommand;
	app_->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::DocumentManager::clearPreviewCommand()
{
	previewCommand_ = nullptr;
	app_->canvas()->update(RedrawMode::fullRedraw);
}

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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolButton>
#include <QDockWidget>
#include <QPushButton>
#include <QResource>
#include <QMessageBox>
#include <QDesktopServices>

#include "application.h"
#include "dialogs/newimagedialog.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/imagepreviewdialog.h"
#include "dialogs/imageresizedialog.h"

MainWindow::MainWindow(a2img::Application* app, QWidget* parent) :
	QMainWindow(parent),
	app_(app),
	ui_(new Ui::MainWindow),
	glView_(new QOpenGLWidget),
	statusMousePositionLabel_(new QLabel)
{
	ui_->setupUi(this);

	ui_->canvasView->setApplicationPointer(app);
	ui_->canvasView->setViewport(glView_.get());
	ui_->canvasView->setScene(app->canvas());
	ui_->canvasBox->layout()->addWidget(ui_->canvasView);

	// Enable mouse move events for the canvas.
	ui_->canvasView->setMouseTracking(true);

	// Setup status bar.
	ui_->statusBar->addWidget(statusMousePositionLabel_);

	// Allow the user to drag images onto the canvas.
	setAcceptDrops(true);

	ui_->documentsTabBar->setTabsClosable(true);
	ui_->documentsTabBar->setMovable(true);
	ui_->documentsTabBar->setExpanding(false);

	QObject::connect(ui_->documentsTabBar, &QTabBar::currentChanged, [=](int index) {
		if(index == -1) {
			return;
		}
		app_->documentManager().setCurrentDocument(index);
	});

	QObject::connect(ui_->documentsTabBar, &QTabBar::tabCloseRequested, [=](int index) {
		if(index == -1) {
			return;
		}
		app_->documentManager().closeDocument(index);
	});

	setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

	loadTheme();
}

MainWindow::~MainWindow()
{
	delete ui_;
}

void MainWindow::loadToolBar()
{
	QActionGroup* toolsGroup = new QActionGroup(this);

	std::vector<a2img::Tool*> tools = app_->tools();
	for(a2img::Tool* tool : tools) {
		QAction* action = ui_->toolBar->addAction(tool->displayName());
		action->setCheckable(true);

		// Recolour the icon based on the theme.
		QImage iconImage(tool->iconPath());
		if(isDarkThemeInUse()) {
			iconImage.invertPixels();
		}
		action->setIcon(QIcon(QPixmap::fromImage(iconImage)));

		QObject::connect(action, &QAction::triggered, [=]() {
			app_->setCurrentTool(tool);
		});

		toolsGroup->addAction(action);
	}

	app_->settingsManager().onValueChanged("userInterface", "toolbarStyle", [=](QVariant value) {
		bool isLarge = value.toString() == "large";
		ui_->toolBar->setToolButtonStyle(isLarge ? Qt::ToolButtonTextUnderIcon : Qt::ToolButtonIconOnly);

		// Ensure the width of each item is consistent.
		for(QAction* action : toolsGroup->actions()) {
			action->associatedWidgets()[1]->setMinimumWidth(isLarge ? 64 : 0);
		}
	});
}

void MainWindow::loadEffectsMenu()
{
	// Delete old menu entries.
	QList<QAction*> oldMenuEntries =
		ui_->menuEffects->actions();
	for(QAction* menuEntry : oldMenuEntries) {
		if(menuEntry->objectName() != "actionEffectsRefresh") {
			ui_->menuEffects->removeAction(menuEntry);
		}
	}

	std::map<QString, QMenu*> categories;

	std::vector<a2img::Effect*> effects = app_->effectManager().effects();
	for(a2img::Effect* effect : effects) {
		// If a menu for this category doesn't exist, create it.
		if(categories.find(effect->category()) == categories.end()) {
			QMenu* menu = new QMenu(effect->category());
			categories[effect->category()] = menu;
			ui_->menuEffects->addMenu(menu);
		}

		// Create the effect menu item.
		QMenu* menu = categories[effect->category()];
		QAction* effectAction = menu->addAction(effect->displayName());

		QObject::connect(effectAction, &QAction::triggered, [=]() {
			if(!app_->documentManager().isDocumentOpen()) {
				return;
			}
			a2img::Document* doc = app_->documentManager().currentDocument();

			std::unique_ptr<a2img::EffectCommand> effectCommand(
				new a2img::EffectCommand(doc->selectedLayer(), effect->package(), effect->name()));
			doc->selectedLayer()->addQueuedCommand(std::move(effectCommand));
		});
	}
}

void MainWindow::loadTheme()
{
	QString baseStyleSheetPath;
	if(isDarkThemeInUse()) {
		// Dark theme.
		baseStyleSheetPath = ":/css/stylesheetDark.css";
	} else {
		// Light theme.
		baseStyleSheetPath = ":/css/stylesheet.css";
	}

	QFile baseStyleSheet(baseStyleSheetPath);
	baseStyleSheet.open(QIODevice::ReadOnly);
	if(!baseStyleSheet.isOpen()) {
		qDebug() << "Failed to load stylesheet resource.";
	}

	QString baseStyleSheetContent = baseStyleSheet.readAll();
	auto updateTheme = [=]() {
		a2img::SettingsManager& config = app_->settingsManager();
		config.beginGroup("userInterface");
		QString themeName = config.value("theme", "Default").toString();
		config.endGroup();

		if(themeName == "Default") {
			setStyleSheet(baseStyleSheetContent);
		} else {
			a2img::Theme theme = app_->themeManager().theme(themeName);
			if(!QResource::registerResource(theme.resourcePath)) {
				qDebug() << "cannot open resource";
			}
			setStyleSheet(baseStyleSheetContent + theme.styleSheet);
		}
	};

	updateTheme();

	app_->settingsManager().onValueChanged("userInterface", "theme", [=](QVariant) {
		updateTheme();
	});
}

void MainWindow::setStatusMousePosition(QPoint mousePosition)
{
	QString xCoord = QString::number(mousePosition.x());
	QString yCoord = QString::number(mousePosition.y());
	statusMousePositionLabel_->setText(QString("Cursor Position: ") + xCoord + ", " + yCoord);
}

bool MainWindow::isDarkThemeInUse()
{
	return palette().color(QPalette::Text).red() > 127;
}

QTabBar* MainWindow::documentTabBar()
{
	return ui_->documentsTabBar;
}

a2img::CanvasView* MainWindow::canvasView()
{
	return ui_->canvasView;
}

//
// Public Slots
//

/*
	File Menu
*/

void MainWindow::on_actionFileNew_triggered()
{
	NewImageDialog* dialog = new NewImageDialog(this);
	int result = dialog->exec();

	if(result == QDialog::Rejected) {
		return;
	}

	app_->documentManager().createDocument(dialog->getDocumentSize(), dialog->getDocumentBackgroundColour());
}

void MainWindow::on_actionFileOpen_triggered()
{
	a2img::ImagePreviewDialog dialog(this);

	// Specify valid file formats.
	const char* filter =
		"Supported File Types [A2, PNG, JPEG, BMP] (*.a2 *.png *.jpg *.jpeg *.bmp);;"
		"a2img Document [A2] (*.a2);;"
		"Portable Network Graphics [PNG] Files (*.png);;"
		"Joint Photographic Experts Group [JPEG] Files (*.jpg *.jpeg);;"
		"Bitmap [BMP] Files (*.bmp)";
	dialog.setNameFilter(filter);

	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);

	// Display the dialog.
	if(!dialog.exec()) {
		return;
	}

	// Load the selected document.
	const QStringList fileNames = dialog.selectedFiles();
	const QString name = fileNames[0];
	app_->documentManager().openDocument(name);
}

void MainWindow::on_actionFileSave_triggered()
{
	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}

	a2img::Document* doc = app_->documentManager().currentDocument();
	QString path = doc->path();

	// If the file hasn't been saved:
	if(path == "/") {
		on_actionFileSaveAs_triggered();
		return;
	}

	doc->save(path, doc->format());
}

void MainWindow::on_actionFileSaveAs_triggered()
{
	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}

	QFileDialog dialog(this);

	const char* filters =
		"a2img Document [A2] (*.a2);;"
		"Portable Network Graphics [PNG] Files (*.png);;"
		"Joint Photographic Experts Group [JPEG] Files (*.jpg *.jpeg);;"
		"Bitmap [BMP] Files (*.bmp)";
	dialog.setNameFilter(filters);

	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptSave);

	// Display the dialog.
	if(!dialog.exec()) {
		return;
	}

	// Load the selected document.
	const QStringList fileNames = dialog.selectedFiles();
	const QString name = fileNames[0];

	QString filter = dialog.selectedNameFilter();
    a2img::ImageFormat format = a2img::ImageFormat::png;
	if(filter[0] == 'a') {
		format = a2img::ImageFormat::a2;
	} else if(filter[0] == 'P') {
        format = a2img::ImageFormat::png;
	} else if(filter[0] == 'J') {
        format = a2img::ImageFormat::jpg;
	} else if(filter[0] == 'B') {
        format = a2img::ImageFormat::bmp;
	}

	app_->documentManager().currentDocument()->save(name, format);
}

void MainWindow::on_actionFileSettings_triggered()
{
	SettingsDialog* dialog = new SettingsDialog(app_, this);
	dialog->exec();
}

/*
	Edit Menu
*/

void MainWindow::on_actionEditUndo_triggered()
{
	if(app_->documentManager().isDocumentOpen()) {
		app_->documentManager().currentDocument()->undo();
	}
}

void MainWindow::on_actionEditRedo_triggered()
{
	if(app_->documentManager().isDocumentOpen()) {
		app_->documentManager().currentDocument()->redo();
	}
}

/*
	View Menu
*/

void MainWindow::on_actionViewZoomIn_triggered()
{
	float zoomPercentage = app_->canvas()->getZoomPercentage();
	if(zoomPercentage < 1000) {
		app_->canvas()->setZoomPercentage(zoomPercentage * 1.25F);
	}
}

void MainWindow::on_actionViewZoomOut_triggered()
{

	float zoomPercentage = app_->canvas()->getZoomPercentage();
	if(zoomPercentage > 10) {
		app_->canvas()->setZoomPercentage(zoomPercentage / 1.25F);
	}
}

void MainWindow::on_actionViewZoom100_triggered()
{
	app_->canvas()->setZoomPercentage(100);
}

void MainWindow::on_actionViewZoomBestFit_triggered()
{
	if(app_->documentManager().isDocumentOpen()) {
		a2img::Document* doc = app_->documentManager().currentDocument();
		QRectF viewport = app_->canvas()->getViewportRectangle();

		float zoomX = viewport.width() / doc->size().width();
		float zoomY = viewport.height() / doc->size().height();

		// Find the largest zoom percentage for which the image is fully visible.
		float newZoomLevel = std::min(zoomX, zoomY) * 100;

		app_->canvas()->setZoomPercentage(newZoomLevel);
	}
}

/*
    Image Menu
*/

void MainWindow::on_actionImageResize_triggered()
{
	ImageResizeDialog* dialog = new ImageResizeDialog(app_);
	dialog->exec();
}

/*
	Effects Menu
*/

void MainWindow::on_actionEffectsRefresh_triggered()
{
	app_->effectManager().reload();
}

/*
	Help Menu
*/

void MainWindow::on_actionHelpAbout_triggered()
{
	AboutDialog* dialog = new AboutDialog(app_, this);
	dialog->exec();
}

void MainWindow::on_actionHelpAboutQt_triggered()
{
	QMessageBox::aboutQt(this);
}

void MainWindow::on_actionHelpReportBug_triggered()
{
	QDesktopServices::openUrl(QUrl(BUG_REPORTING_URL));
}

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QLabel>
#include <QTreeView>

#include "canvasview.h"

namespace Ui {
	class MainWindow;
}

namespace a2img {
    class Application;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    explicit MainWindow(a2img::Application* app, QWidget* parent = 0);
	~MainWindow();

	// Populate tool bar.
	void loadToolBar();
	// Populate list of effects.
	void loadEffectsMenu();

	void loadTheme();

	// Set the mouse position displayed on the status bar.
	void setStatusMousePosition(QPoint mousePosition);

	bool isDarkThemeInUse();

	QTabBar* documentTabBar();
	a2img::CanvasView* canvasView();

private slots:
	/*
		File Menu
	*/

	void on_actionFileNew_triggered();
	void on_actionFileOpen_triggered();
	void on_actionFileSave_triggered();
	void on_actionFileSaveAs_triggered();
	void on_actionFileSettings_triggered();

	/*
		Edit Menu
	*/

	void on_actionEditUndo_triggered();
	void on_actionEditRedo_triggered();

	/*
		View Menu
	*/

	void on_actionViewZoomIn_triggered();
	void on_actionViewZoomOut_triggered();
	void on_actionViewZoom100_triggered();
	void on_actionViewZoomBestFit_triggered();

	/*
		Effects Menu
	*/

	void on_actionEffectsRefresh_triggered();

	/*
		Help Menu
	*/

	void on_actionHelpAbout_triggered();
	void on_actionHelpAboutQt_triggered();
	void on_actionHelpReportBug_triggered();

    void on_actionImageResize_triggered();

private:

    a2img::Application* app_;

	Ui::MainWindow* ui_;

	std::unique_ptr<QOpenGLWidget> glView_;

	QLabel* statusMousePositionLabel_;
};

#endif // MAINWINDOW_H

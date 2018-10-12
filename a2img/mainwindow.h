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

#ifndef APPLICATION_H
#define APPLICATION_H

#include "buildconfig.h"

#include <vector>
#include <memory>

#include <QApplication>

#include "mainwindow.h"
#include "canvas.h"
#include "colour/colourmanager.h"
#include "effectmanager.h"
#include "document/document.h"
#include "tools/tool.h"
#include "maskmanager.h"
#include "versionnumber.h"
#include "document/documentmanager.h"
#include "thememanager.h"
#include "settingsmanager.h"

namespace a2img {
    class Application : public QApplication
    {
    public:
        Application(int& argc, char** argv);

		SettingsManager& settingsManager() { return settingsManager_; }
        MainWindow* window() { return &window_; }
        Canvas* canvas() { return &canvas_; }
		ThemeManager& themeManager() { return themeManager_; }
		ColourManager& colourManager() { return colourManager_; }
		EffectManager& effectManager() { return effectManager_; }
		MaskManager& maskManager() { return maskManager_; }
		DocumentManager& documentManager() { return documentManager_; }


		// Returns a vector of pointers to tool objects.
		std::vector<Tool*> tools();
		// Sets the currently selected tool.
		void setCurrentTool(Tool* selectedTool);

	protected:
		void loadTools();

	private:
		SettingsManager settingsManager_;
		Canvas canvas_;
		ThemeManager themeManager_;
		DocumentManager documentManager_;
		MainWindow window_;
		ColourManager colourManager_;
		EffectManager effectManager_;
		MaskManager maskManager_;

		std::vector<std::unique_ptr<Tool>> tools_;
		std::vector<std::unique_ptr<Tool>>::iterator currentTool_;
    };
}

#endif // APPLICATION_H

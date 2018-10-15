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
#include "shadermanager.h"

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


		ShaderManager shaderManager;

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

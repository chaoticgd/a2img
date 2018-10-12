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

#include "application.h"

#include <iostream>
#include <QStringList>

#include "vectorutil.h"
#include "tools/viewtool.h"
#include "tools/selectiontool.h"
#include "tools/lassotool.h"
#include "tools/brushtool.h"
#include "tools/filltool.h"
#include "tools/texttool.h"

int main(int argc, char* argv[])
{
	a2img::Application app(argc, argv);

	auto arguments = app.arguments();
	arguments.removeFirst();
	for(QString argument : arguments) {
		if(argument == "--help") {
			std::cout << "a2img: Free and open source image editor." << std::endl;
			std::cout << "To file a bug report, please see: " << BUG_REPORTING_URL << std::endl;
			return 0;
		} else if(argument == "--version") {
			std::cout << "Application Version: " << APPLICATION_VERSION.toString().toStdString() << std::endl;
			std::cout << "Effects API Version: " << EFFECTS_API_VERSION.toString().toStdString() << std::endl;
			std::cout << "File Format Version: " << FILE_FORMAT_VERSION.toString().toStdString() << std::endl;
			return 0;
		} else {
			app.documentManager().openDocument(argument);
		}
	}

	return app.exec();
}

#define BASE_PATH ".."

a2img::Application::Application(int& argc, char** argv)
	: QApplication(argc, argv),
	  settingsManager_(BASE_PATH),
	  canvas_(this),
	  themeManager_(BASE_PATH),
	  documentManager_(this),
	  window_(this),
	  colourManager_(this),
	  effectManager_(this),
	  maskManager_(this),
	  currentTool_(tools_.end())
{
	setApplicationDisplayName("a2img");

	loadTools();

	window_.show();

	documentManager_.loadLayersPanel();
	documentManager_.updateUserInterface();
	settingsManager_.update();
}

std::vector<a2img::Tool*> a2img::Application::tools()
{
	return uniqueVectorToRaw<Tool>(tools_);
}

void a2img::Application::setCurrentTool(Tool* selectedTool)
{
	// If the new tool is already selected, do nothing.
	if(currentTool_->get() == selectedTool) {
		return;
	}

	/*auto oldTool = std::find(tools_.begin(), tools_.end(), *currentTool_);
	(*oldTool)->disabled();

	auto newTool = std::find_if(
		tools_.begin(), tools_.end(),
		[=](std::unique_ptr<Tool>& current) { return current.get() == selectedTool; }
	);
	(*newTool)->enabled();*/

	auto previousTool = currentTool_;
	for(auto tool = tools_.begin(); tool < tools_.end(); tool++) {
		if(tool->get() == selectedTool) {
			// Enable the newly selected tool
			currentTool_ = tool;
			tool->get()->enabled();
		} else if(tool == previousTool) {
			// Disable the previously selected tool.
			tool->get()->disabled();
		}
	}
}

void a2img::Application::loadTools()
{
	// Populate the list of tools.
	tools_.emplace_back(new ViewTool(this));
	tools_.emplace_back(new SelectionTool(this));
	//tools_.emplace_back(new LassoTool(this));
	tools_.emplace_back(new BrushTool(this));
	tools_.emplace_back(new FillTool(this));
	//tools_.emplace_back(new TextTool(this));

	// Display these in the UI.
	window_.loadToolBar();

	// Set the current tool to the ViewTool.
	currentTool_ = tools_.begin();
	currentTool_->get()->enabled();
}

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

#include "viewtool.h"

#include <QGraphicsView>

#include "application.h"

a2img::ViewTool::ViewTool(Application* app)
	: Tool(app),
	  miniMap_(new MiniMap(app))
{
}

QString a2img::ViewTool::displayName()
{
	return "View";
}

QString a2img::ViewTool::iconPath()
{
	return ":/icons/viewTool.svg";
}

void a2img::ViewTool::enabled()
{
	app()->canvas()->addItem(miniMap_);
	app()->canvas()->views()[0]->setDragMode(QGraphicsView::ScrollHandDrag);
}

void a2img::ViewTool::disabled()
{
	app()->canvas()->removeItem(miniMap_);
	app()->canvas()->views()[0]->setDragMode(QGraphicsView::NoDrag);
}

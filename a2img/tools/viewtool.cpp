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

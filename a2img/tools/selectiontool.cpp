#include "selectiontool.h"

#include <cmath>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDateTime>
#include <QTimer>
#include <QOpenGLFramebufferObject>

#include "application.h"
#include "util/rectangle.h"

a2img::SelectionTool::SelectionTool(Application* app)
	: SelectionBoxTool(app)
{}

QString a2img::SelectionTool::displayName()
{
	return "Select";
}

QString a2img::SelectionTool::iconPath()
{
	return ":/icons/selectionTool.svg";
}

void a2img::SelectionTool::enabled()
{
	app()->canvas()->addItem(this);

	SelectionBoxTool::enabled();
}

void a2img::SelectionTool::disabled()
{
	app()->canvas()->removeItem(this);

	SelectionBoxTool::disabled();
}

void a2img::SelectionTool::selectionBoxReleased(Rectangle<int> rect)
{
	Document* doc = app()->documentManager().currentDocument();

	doc->setSelectedRegion(rect);
}

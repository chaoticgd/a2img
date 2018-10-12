#include "canvasview.h"

#include <QWheelEvent>

#include "application.h"

a2img::CanvasView::CanvasView(QWidget* parent)
	: QGraphicsView(parent),
	  isDragging_(false)
{}

void a2img::CanvasView::setApplicationPointer(Application* app)
{
	app_ = app;
}

void a2img::CanvasView::wheelEvent(QWheelEvent* event)
{
	QGraphicsView::wheelEvent(event);
}

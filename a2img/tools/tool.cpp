#include "tool.h"

#include <limits.h>

#include "application.h"

a2img::Tool::Tool(Application* app)
	: app_(app)
{}

QRectF a2img::Tool::boundingRect() const
{
	if(!app_->documentManager().isDocumentOpen()) {
		return QRectF(0, 0, 0, 0);
	}
	float zoom = app_->canvas()->getZoomPercentage() / 100;
	QSize size = app_->documentManager().currentDocument()->size() * zoom;
	return QRectF(0, 0, size.width(), size.height());
}

a2img::Application* a2img::Tool::app()
{
	return app_;
}

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

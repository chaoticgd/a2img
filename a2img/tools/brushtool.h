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

#ifndef BRUSHTOOL_H
#define BRUSHTOOL_H

#include <vector>
#include <memory>

#include <QGraphicsItem>
#include <QPointF>
#include <QToolBar>
#include <QRectF>

#include "tool.h"
#include "../colour/colourmanager.h"
#include "../maskmanager.h"
#include "../commands/brushcommand.h"

namespace a2img {
	class Application;

	class BrushTool : public Tool
	{
	public:
		BrushTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

	protected:
		void hoverEnterEvent(QGraphicsSceneHoverEvent*);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
		void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

		void loadToolBar();

	private:
		int diameter_;
		BlendMode blendMode_;
		std::unique_ptr<BrushCommand> brushCommand_;

		bool isHovering_;
		QPointF lastPosition_;

		QToolBar* toolBar_;

	};
}

#endif // BRUSHTOOL_H

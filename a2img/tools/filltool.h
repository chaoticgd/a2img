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

#ifndef FILLTOOL_H
#define FILLTOOL_H

#include <QToolBar>
#include <QGraphicsItem>

#include "tool.h"
#include "../commands/fillcommand.h"

namespace a2img {
	class Application;

	class FillTool : public Tool
	{
	public:
		FillTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();

		void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

	protected:
		void hoverEnterEvent(QGraphicsSceneHoverEvent*);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent*);
		void hoverMoveEvent(QGraphicsSceneHoverEvent* event);

		void mousePressEvent(QGraphicsSceneMouseEvent*event);

		void loadToolBar();

	private:
		QToolBar* toolBar_;
		int tolerance_;
		bool globalFill_;
		std::unique_ptr<FillCommand> fillCommand_;
	};
}

#endif // FILLTOOL_H

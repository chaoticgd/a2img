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

#ifndef SELECTIONBOXTOOL_H
#define SELECTIONBOXTOOL_H

#include "tool.h"
#include "util/rectangle.h"

namespace a2img {
	class SelectionBoxTool : public Tool
	{
	protected:
		SelectionBoxTool(Application* app);

	public:
		void enabled();
		void disabled();

		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		bool isSelecting();

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

		virtual void selectionBoxChanged(Rectangle<int>) {}
		virtual void selectionBoxReleased(Rectangle<int>) {}

	private:
		Rectangle<int> selectionRectangle_;
		std::vector<std::function<void(Vec2<int>)>> dragHandlers_;
	};
}

#endif // SELECTIONBOXTOOL_H

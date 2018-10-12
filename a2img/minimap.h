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

#ifndef MINIMAP_H
#define MINIMAP_H

#include <QGraphicsItem>
#include <QPainter>

#include "application.h"

namespace a2img {
	// Creates a minimap in the bottom-right hand
	// corner of the canvas when the entire image
	// can't be displayed at once.
	class MiniMap : public QGraphicsItem
	{
	public:
		MiniMap(Application* app);

		QRectF boundingRect() const;
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		void mousePressEvent(QGraphicsSceneMouseEvent* event);

		// Bound the relativePosition passed
		// to within the outer rectangle.
		// Return value is within [(0, 0), (1, 1)].
		QPointF getBoundedPosition(QPointF relativePosition) const;

		// Calculate the coordinates of the
		// rectangle representing the document.
		QRectF getOuterRectangle() const;

		// Calculate the coordinates of the,
		// rectangle representing the viewport.
		QRectF getInnerRectangle() const;
	private:
		Application* app_;
	};
}

#endif // MINIMAP_H

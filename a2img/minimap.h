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

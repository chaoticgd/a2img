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

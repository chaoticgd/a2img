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

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

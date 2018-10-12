#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QGraphicsItem>
#include <QOpenGLShaderProgram>

#include "selectionboxtool.h"

namespace a2img {
	class Application;

	class SelectionTool : public SelectionBoxTool
	{
	public:
		SelectionTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();

	protected:
		virtual void selectionBoxReleased(Rectangle<int> rect);
	};
}

#endif // SELECTIONTOOL_H

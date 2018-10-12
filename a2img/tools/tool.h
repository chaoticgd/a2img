#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <QKeySequence>
#include <QGraphicsItem>

namespace a2img {
	class Application;

	// Pure virtual base class that all tools inherit from.
	class Tool : public QGraphicsItem
	{
	protected:
		Tool(Application* app);
	public:

		QRectF boundingRect() const;
		void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) {}


		// Returns the text to be displayed next to the tools icon.
		virtual QString displayName() = 0;

		// Returns the text resource path of the tools icon.
		virtual QString iconPath() = 0;


		// Called when the tool is selected by the user.
		virtual void enabled() = 0;

		// Called when the tool is deselected by the user.
		virtual void disabled() = 0;

	protected:
		Application* app();

	private:
		Application* app_;
	};
}

#endif // TOOL_H

#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QToolBar>

#include "selectionboxtool.h"
#include "../commands/textcommand.h"

namespace a2img {
	class TextTool : public SelectionBoxTool
	{
	public:
		TextTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();

	protected:
		void keyPressEvent(QKeyEvent* event);

		void selectionBoxChanged(Rectangle<int> documentRect);

	private:
		void loadToolBar();

		QToolBar* toolBar_;
		std::unique_ptr<TextCommand> textCommand_;
	};
}

#endif // TEXTTOOL_H

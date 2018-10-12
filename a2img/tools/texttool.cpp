#include "texttool.h"

#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

#include "application.h"

a2img::TextTool::TextTool(Application* app)
	: SelectionBoxTool(app)
{
	loadToolBar();
}

QString a2img::TextTool::displayName()
{
	return "Text";
}

QString a2img::TextTool::iconPath()
{
	return ":/icons/textTool.svg";
}

void a2img::TextTool::enabled()
{
	app()->canvas()->addItem(this);
	toolBar_->show();
	textCommand_.reset(new TextCommand(app()->documentManager().currentDocument()->selectedLayer()));
	app()->documentManager().setPreviewCommand(textCommand_.get());

	SelectionBoxTool::enabled();
}

void a2img::TextTool::disabled()
{
	app()->canvas()->removeItem(this);
	toolBar_->hide();
	app()->documentManager().clearPreviewCommand();

	SelectionBoxTool::disabled();
}

void a2img::TextTool::keyPressEvent(QKeyEvent* event)
{
	QString text = textCommand_->text();

	if(event->key() == Qt::Key_Backspace) {
		text = text.mid(0, text.size() - 1);
	} else if(event->key() >= Qt::Key_A && event->key() <= Qt::Key_Z) {
		if(event->modifiers().testFlag(Qt::ShiftModifier)) {
			text += event->key() - Qt::Key_A + 'A';
		} else {
			text += event->key() - Qt::Key_A + 'a';
		}
	}

	textCommand_->setText(text);
}

void a2img::TextTool::selectionBoxChanged(Rectangle<int> documentRect)
{

}

void a2img::TextTool::loadToolBar()
{
	toolBar_ = new QToolBar;
	toolBar_->setWindowTitle("Text Options");
	app()->window()->addToolBar(toolBar_);
}

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

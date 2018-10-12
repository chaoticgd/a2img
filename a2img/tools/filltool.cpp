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

#include "filltool.h"

#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QSlider>
#include <QComboBox>

#include "application.h"
#include "../commands/floodfillcommand.h"
#include "../commands/globalfillcommand.h"
#include "../sliderbox.h"

a2img::FillTool::FillTool(Application* app)
	: Tool(app),
	  tolerance_(32),
	  globalFill_(false)
{
	loadToolBar();
	setAcceptHoverEvents(true);
}

QString a2img::FillTool::displayName()
{
	return "Fill";
}

QString a2img::FillTool::iconPath()
{
	return ":/icons/fillTool.svg";
}

void a2img::FillTool::enabled()
{
	toolBar_->show();
	app()->canvas()->addItem(this);
}

void a2img::FillTool::disabled()
{
	toolBar_->hide();
	app()->canvas()->removeItem(this);
}

void a2img::FillTool::paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
{
}

void a2img::FillTool::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
	QColor colour = app()->colourManager().getColour();
	Layer* selectedLayer = app()->documentManager().currentDocument()->selectedLayer();
	if(globalFill_) {
		fillCommand_.reset(new GlobalFillCommand(selectedLayer, colour, tolerance_));
	} else {
		fillCommand_.reset(new FloodFillCommand(selectedLayer, colour, tolerance_));
	}

	app()->documentManager().setPreviewCommand(fillCommand_.get());
}

void a2img::FillTool::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
	app()->documentManager().clearPreviewCommand();
	fillCommand_.reset(nullptr);
}

void a2img::FillTool::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	if(fillCommand_.get() == nullptr) {
		return;
	}

	Document* doc = app()->documentManager().currentDocument();
	fillCommand_->setStartingPoint(doc->mapSceneToDocument(event->pos()));
	app()->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::FillTool::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if(!app()->documentManager().isDocumentOpen() || fillCommand_.get() == nullptr) {
		return;
	}

	app()->documentManager().clearPreviewCommand();

	Document* doc = app()->documentManager().currentDocument();
	doc->pushUndoCommand(fillCommand_.release());
	hoverEnterEvent(nullptr);
}

void a2img::FillTool::loadToolBar()
{
	toolBar_ = new QToolBar;
	toolBar_->setWindowTitle("Fill Options");
	toolBar_->hide();
	app()->window()->addToolBar(toolBar_);

	toolBar_->addWidget(new QLabel("    Tolerance "));

	SliderBox* toleranceInput = new SliderBox(new QSlider(Qt::Horizontal), new QSpinBox);
	toleranceInput->setRange(1, 255, 1, 255);
	toleranceInput->setValue(32);
	toleranceInput->slider()->setMaximumWidth(128);
	toleranceInput->spinBox()->setMaximumWidth(64);
	toolBar_->addWidget(toleranceInput);

	QObject::connect(toleranceInput->slider(), &QSlider::valueChanged, [=](int tolerance) {
		tolerance_ = tolerance;
	});

	toolBar_->addWidget(new QLabel("    Mode "));
	QComboBox* modeBox = new QComboBox;
	modeBox->addItem("Flood");
	modeBox->addItem("Global");
	QObject::connect(
		modeBox,
		&QComboBox::currentTextChanged,
		[=](QString text) {
			globalFill_ = text == "Global";
		}
	);
	toolBar_->addWidget(modeBox);
}

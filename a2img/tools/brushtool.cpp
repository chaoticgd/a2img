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

#include "brushtool.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QAction>

#include "../application.h"

a2img::BrushTool::BrushTool(Application* app)
	: Tool(app),
	  diameter_(4),
	  blendMode_(BlendMode::normal),
	  isHovering_(false)
{
	loadToolBar();
	setAcceptHoverEvents(true);
}

QString a2img::BrushTool::displayName()
{
	return "Brush";
}

QString a2img::BrushTool::iconPath()
{
	return ":/icons/brushTool.svg";
}

void a2img::BrushTool::enabled()
{
	app()->canvas()->addItem(this);
	toolBar_->show();
}

void a2img::BrushTool::disabled()
{
	app()->canvas()->removeItem(this);
	toolBar_->hide();
}

void a2img::BrushTool::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(!isHovering_) {
		return;
	}

    // Draw circular marker.
	Document* doc = app()->documentManager().currentDocument();
	QPointF centre = doc->mapDocumentToScene((lastPosition_));
	float zoom = app()->canvas()->getZoomPercentage() / 100;
	double radius = (diameter_ * zoom) / 2;

	painter->setPen(QColor(0, 0, 0, 63));
	painter->drawEllipse(centre, radius - 1, radius - 1);
	painter->drawEllipse(centre, radius + 1, radius + 1);

	painter->setPen(QColor(255, 255, 255, 127));
	painter->drawEllipse(centre, radius, radius);
}

void a2img::BrushTool::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
	isHovering_ = true;
}

void a2img::BrushTool::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
	isHovering_ = false;
	app()->canvas()->update(RedrawMode::onlyRedrawForeground);
}

void a2img::BrushTool::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	Document* doc = app()->documentManager().currentDocument();
	lastPosition_ = doc->mapSceneToDocument(event->pos());
	app()->canvas()->update(RedrawMode::onlyRedrawForeground);
}

void a2img::BrushTool::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	Document* doc = app()->documentManager().currentDocument();
	lastPosition_ = doc->mapSceneToDocument(event->pos());

	if(brushCommand_.get() == nullptr) {
		return;
	}

	brushCommand_->appendPoint(doc->mapSceneToDocument(event->pos()));
	app()->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::BrushTool::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	Document* doc = app()->documentManager().currentDocument();

	if(event->button() != Qt::LeftButton ||
		!doc->isSelectedLayerValid()) {
		return;
	}

	brushCommand_.reset(new BrushCommand(
		doc->selectedLayer(), diameter_,
		app()->colourManager().getColour(), blendMode_
	));
	brushCommand_->appendPoint(doc->mapSceneToDocument(event->pos()));
	app()->documentManager().setPreviewCommand(brushCommand_.get());
}

void a2img::BrushTool::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
	if(brushCommand_.get() == nullptr) {
		return;
    }
    app()->documentManager().clearPreviewCommand();
    Document* doc = brushCommand_->document();
    doc->pushUndoCommand(brushCommand_.release());
}

void a2img::BrushTool::loadToolBar()
{
	toolBar_ = new QToolBar;
	toolBar_->setWindowTitle("Brush Options");
	toolBar_->hide();
	app()->window()->addToolBar(toolBar_);

    // Create diameter input field.
	toolBar_->addWidget(new QLabel("    Diameter "));

	QSpinBox* diameterBox = new QSpinBox;
	diameterBox->setRange(1, 999);
	diameterBox->setValue(4);
	diameterBox->setSuffix("px");
	QObject::connect(
		diameterBox,
		static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[=](int newValue) {
			diameter_ = newValue;
		}
	);
	toolBar_->addWidget(diameterBox);

    // Create mode input field.
	toolBar_->addWidget(new QLabel("    Mode "));
	QComboBox* modeBox = new QComboBox;
	modeBox->addItem("Normal");
	modeBox->addItem("Highlighter");
	QObject::connect(
		modeBox,
		&QComboBox::currentTextChanged,
		[=](QString text) {
			if(text == "Highlighter") {
				blendMode_ = BlendMode::multiply;
			} else {
				blendMode_ = BlendMode::normal;
			}
		}
	);
	toolBar_->addWidget(modeBox);
}

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

#include "selectionboxtool.h"

#include <QGraphicsSceneMouseEvent>
#include <QDateTime>

#include "application.h"
#include "../util/serialisation.h"

a2img::SelectionBoxTool::SelectionBoxTool(Application* app)
	: Tool(app)
{
}

void a2img::SelectionBoxTool::enabled()
{
}

void a2img::SelectionBoxTool::disabled()
{
}

void a2img::SelectionBoxTool::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(selectionRectangle_.size().x == 0 ||
	   selectionRectangle_.size().y == 0) {
		return;
	}

	Document* doc = app()->documentManager().currentDocument();

	QRectF selectionBox = doc->mapDocumentToScene(selectionRectangle_);

	QPen pen;

	// Draw border shadow.
	pen.setColor(QColor(100, 100, 100, 200));
	pen.setWidth(3);
	painter->setPen(pen);
	painter->drawRect(selectionBox);

	// Draw grab handle boundaries.
	QPointF position = selectionBox.topLeft();
	QSizeF size = selectionBox.size();
	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawLine(
		QPointF { position.x() + size.width() / 3.0, position.y() },
		QPointF { position.x() + size.width() / 3.0, position.y() + size.height() }
	);
	painter->drawLine(
		QPointF { position.x() + size.width() / 1.5, position.y() },
		QPointF { position.x() + size.width() / 1.5, position.y() + size.height() }
	);
	painter->drawLine(
		QPointF { position.x(), position.y() + size.height() / 3.0 },
		QPointF { position.x() + size.width(), position.y() + size.height() / 3.0 }
	);
	painter->drawLine(
		QPointF { position.x(), position.y() + size.height() / 1.5 },
		QPointF { position.x() + size.width(), position.y() + size.height() / 1.5 }
	);

	// Draw border.
	pen.setColor(QColor(255, 255, 255, 255));
	pen.setDashPattern({ 4, 4 });
	pen.setDashOffset((QDateTime::currentMSecsSinceEpoch() % 16) / 2);
	painter->setPen(pen);
	painter->drawRect(selectionBox);

	std::vector<Vec2<int>> markers;
	markers.push_back(selectionRectangle_.topLeft());
	markers.push_back(selectionRectangle_.topRight());
	markers.push_back(selectionRectangle_.bottomLeft());
	markers.push_back(selectionRectangle_.bottomRight());

	for(Vec2<int> marker : markers) {
		QString text = serialisation::toString(marker);
		QPointF scenePos = doc->mapDocumentToScene(marker);

		// Draw text shadow.
		pen.setColor(QColor(0, 0, 0));
		painter->setPen(pen);
		painter->drawText(scenePos + QPointF(1, 1), text);
		pen.setColor(QColor(255, 255, 255));
		painter->setPen(pen);
		painter->drawText(scenePos, text);
	}

	std::vector<std::pair<QPointF, int>> sides {
		{
			(selectionBox.bottomLeft() + selectionBox.bottomRight()) / 2,
			selectionRectangle_.size().x
		},
		{
			(selectionBox.bottomLeft() + selectionBox.topLeft()) / 2,
			selectionRectangle_.size().y
		}
	};

	for(auto side : sides) {
		QString text = QString::number(side.second);
		pen.setColor(QColor(0, 0, 0));
		painter->setPen(pen);
		painter->drawText(side.first + QPointF(1, 1), text);
		pen.setColor(QColor(255, 255, 255));
		painter->setPen(pen);
		painter->drawText(side.first, text);
	}
}

bool a2img::SelectionBoxTool::isSelecting()
{
	return selectionRectangle_.size().x != 0 && selectionRectangle_.size().y != 0;
}

void a2img::SelectionBoxTool::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	Document* doc = app()->documentManager().currentDocument();

	if(dragHandlers_.size() > 0) {
		// Resize an existing selection.
		for(auto handler : dragHandlers_) {
			handler(doc->mapSceneToDocument(event->pos()));
		}
		app()->canvas()->update(RedrawMode::fullRedraw);
		return;
	}

	// Resize a new selection.
	Vec2<int> newPoint = doc->mapSceneToDocument(event->pos());
	selectionRectangle_ = {
		selectionRectangle_.x1, selectionRectangle_.y1,
		newPoint.x, newPoint.y
	};

	selectionBoxChanged(selectionRectangle_);

	app()->canvas()->update(RedrawMode::fullRedraw);
}

void a2img::SelectionBoxTool::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	Document* doc = app()->documentManager().currentDocument();

	Rectangle<float> boundingRect(selectionRectangle_);
	Vec2<float> clickedPos = doc->mapSceneToDocument<float>(event->pos());

	if(boundingRect.contains(clickedPos)) {

		Vec2<float> box = clickedPos - boundingRect.position();

		bool moving = true;

		if(box.x < boundingRect.size().x / 3.0f) {
			int difference = static_cast<int>(box.x);
			dragHandlers_.push_back([=](Vec2<int> point) {
				selectionRectangle_.x1 = point.x - difference;
			});
			moving = false;
		} else if(box.x > boundingRect.size().x / 1.5f) {
			int difference = static_cast<int>(selectionRectangle_.size().x - box.x);
			dragHandlers_.push_back([=](Vec2<int> point) {
				selectionRectangle_.x2 = point.x + difference;
			});
			moving = false;
		}

		if(box.y < boundingRect.size().y / 3.0f) {
			int difference = static_cast<int>(box.y);
			dragHandlers_.push_back([=](Vec2<int> point) {
				selectionRectangle_.y1 = point.y - difference;
			});
			moving = false;
		} else if(box.y > boundingRect.size().y / 1.5f) {
			int difference = static_cast<int>(selectionRectangle_.size().y - box.y);
			dragHandlers_.push_back([=](Vec2<int> point) {
				selectionRectangle_.y2 = point.y + difference;
			});
			moving = false;
		}

		if(moving) {
			dragHandlers_.push_back([=](Vec2<int> point) {
				selectionRectangle_= selectionRectangle_.moved(point - Vec2<int>(box));
			});
		}

		return;
	}

	Vec2<int> startingPoint = doc->mapSceneToDocument(event->pos()).toPoint();
	selectionRectangle_ = {
		startingPoint.x, startingPoint.y,
		startingPoint.x, startingPoint.y
	};
}

void a2img::SelectionBoxTool::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
	dragHandlers_.clear();

	// Make sure x2 > x1, y2 > y1. Otherwise, the drag handlers would break.
	selectionRectangle_ = selectionRectangle_.normalised();

	selectionBoxReleased(selectionRectangle_);
}

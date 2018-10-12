#include "minimap.h"

#include <QGraphicsView>
#include <QScrollBar>
#include <QGraphicsSceneMouseEvent>

a2img::MiniMap::MiniMap(Application* app)
	: QGraphicsItem(0),
	  app_(app)
{
}

QRectF a2img::MiniMap::boundingRect() const
{
	if(!app_->documentManager().isDocumentOpen()) {
		return QRectF(0, 0, 0, 0);
	}
	return getOuterRectangle();
}

void a2img::MiniMap::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(!app_->documentManager().isDocumentOpen()) {
		return;
	}

	QSize documentSize = app_->documentManager().currentDocument()->size();
	const QSizeF viewportSize = app_->canvas()->getViewportRectangle().size();

	// If the whole image can fit on screen, do not show the minimap.
	if(documentSize.width() < viewportSize.width() ||
	   documentSize.height() < viewportSize.height()) {
		return;
	}

	QRectF rectangle = getOuterRectangle();
	painter->fillRect(rectangle, QColor(0, 0, 0, 128));
	painter->setPen(QColor(0, 0, 0));
	painter->drawRect(rectangle);

	QRectF innerRectangle = getInnerRectangle();
	painter->setPen(QColor(255, 255, 255));
	painter->drawRect(innerRectangle);
}

void a2img::MiniMap::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	mousePressEvent(event);
}

void a2img::MiniMap::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QPointF regularPos = getBoundedPosition(event->scenePos() - getOuterRectangle().topLeft());

	QGraphicsView* view = app_->canvas()->views()[0];
	auto scrollBarX = view->horizontalScrollBar();
	auto scrollBarY = view->verticalScrollBar();

	scrollBarX->setValue(regularPos.x() * scrollBarX->maximum());
	scrollBarY->setValue(regularPos.y() * scrollBarY->maximum());

	// Update the bounding rect.
	prepareGeometryChange();
}

QPointF a2img::MiniMap::getBoundedPosition(QPointF relativePosition) const
{
	QRectF outerRectangle = getOuterRectangle();
	QPointF regularPos {
		relativePosition.x() / outerRectangle.size().width(),
		relativePosition.y() / outerRectangle.size().height()
	};

	// Bound inputs outside of the valid range.
	if(regularPos.x() < 0) regularPos.setX(0);
	if(regularPos.x() > 1) regularPos.setX(1);
	if(regularPos.y() < 0) regularPos.setY(0);
	if(regularPos.y() > 1) regularPos.setY(1);

	return regularPos; // (0, 0) <= (x, y) <= (1, 1)
}

QRectF a2img::MiniMap::getOuterRectangle() const
{
	const int maxWidth = 200;
	const int maxHeight = 200;

	const int bottomRightOffset = 8;

	Document* doc = app_->documentManager().currentDocument();
	const QRectF viewportRect = app_->canvas()->getViewportRectangle();

	QRectF rectangle;

	// Determine the size and position of the outer rectangle.
	if(doc->size().width() > doc->size().height()) {
		float documentHeight = doc->size().height();
		float actualHeight = (documentHeight / doc->size().width()) * maxHeight;
		rectangle = QRectF {
			viewportRect.width() - maxWidth - bottomRightOffset,
			viewportRect.height() - actualHeight - bottomRightOffset,
			maxWidth, actualHeight
		};
	} else {
		float documentWidth = doc->size().width();
		float actualWidth = (documentWidth / doc->size().height()) * maxWidth;
		rectangle = QRectF {
			viewportRect.width() - actualWidth - bottomRightOffset,
			viewportRect.height() - maxHeight - bottomRightOffset,
			actualWidth, maxHeight
		};
	}

	// Make the outer rect fixed relative to the viewport.
	QGraphicsView* view = app_->canvas()->views()[0];
	QPoint offset = view->mapFromScene(QPointF(0, 0));
	rectangle.translate(-offset.x(), -offset.y());

	return rectangle;
}

QRectF a2img::MiniMap::getInnerRectangle() const
{
	QGraphicsView* view = app_->canvas()->views()[0];
	auto scrollBarX = view->horizontalScrollBar();
	auto scrollBarY = view->verticalScrollBar();
	QPointF regularCentre {
		static_cast<float>(scrollBarX->value()) / scrollBarX->maximum(),
		static_cast<float>(scrollBarY->value()) / scrollBarY->maximum()
	};

	QRectF outer = getOuterRectangle();
	QRectF viewport = app_->canvas()->getViewportRectangle();
	Document* doc = app_->documentManager().currentDocument();
	float zoom = app_->canvas()->getZoomPercentage() / 100;

	QSizeF size {
		outer.size().width() * viewport.size().width() / (doc->size().width() * zoom),
		outer.size().height() * viewport.size().height() / (doc->size().height() * zoom)
	};
	QRectF outerPadded {
		outer.x() + size.width() / 2, outer.y() + size.height() / 2,
		outer.width() - size.width(), outer.height() - size.height()
	};

	QPointF position {
		outerPadded.x() + outerPadded.size().width() * regularCentre.x(),
		outerPadded.y() + outerPadded.size().height() * regularCentre.y(),
	};


	return QRectF {
		position.x() - size.width() / 2,
		position.y() - size.height() / 2,
		size.width(),
		size.height()
	};
}

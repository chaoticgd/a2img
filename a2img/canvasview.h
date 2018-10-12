#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>

#include "canvas.h"

namespace a2img {
	class Application;

	class CanvasView : public QGraphicsView
	{
	public:
		CanvasView(QWidget* parent = nullptr);

		void setApplicationPointer(Application* app);

	protected:
		void wheelEvent(QWheelEvent* event) override;

	private:
		Application* app_;

		bool isDragging_;
		ViewportData startingViewport_;
		QPointF startPosition_;
	};
}

#endif // CANVASVIEW_H

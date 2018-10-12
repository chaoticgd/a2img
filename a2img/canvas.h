#ifndef CANVAS_H
#define CANVAS_H

#include <deque>
#include <functional>
#include <memory>

#include <QGraphicsScene>
#include <QOpenGLShaderProgram>
#include <QTimer>

#include "glfunctions.h"
#include "texture.h"
#include "util/vec2.h"

typedef std::function<void(a2img::GlFunctions*)> DrawCallback;

namespace a2img {
	class Application;

	enum RedrawMode {
		fullRedraw,
		onlyRedrawForeground
	};

	struct ViewportData {
		QPointF centre;
		float zoom;
	};

    class Canvas : public QGraphicsScene
    {
    public:
		Canvas(Application* app);

		void update(RedrawMode mode);

        // Request a draw operation.
        // callback: Will be called with a valid OpenGL context setup.
        void draw(DrawCallback callback);

		// Returns the percentage zoom.
		float getZoomPercentage();
		// Sets the percentage zoom, and transitions from the old value.
		void setZoomPercentage(float newZoomPercentage);

        // Returns the size the viewport was the last time drawBackground was called.
		QRectF getViewportRectangle();

		// Automatically updates the canvas 30 times a second if enabled.
		void beginLiveUpdates();
		void endLiveUpdates();

    protected:
		// Allow users to drag image over the canvas to open it.
		void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

        // Draw the image, handles zooming, etc.
		void drawBackground(QPainter* painter, const QRectF& rect);

		// Handles callbacks registered by the Canvas::draw function.
		// Only to be called with a valid OpenGL context setup.
		void handleDrawRequests();

		// Creates a texture from the current image.
		GLuint renderLayersToTexture();

		void drawDocument(GLuint layersTexture, QRectF rect);

		// Draws a chequer board behind the image to indicate transparency.
		void drawChequerBoard(QPointF scrollPosition, std::function<void()> drawArrays);

		// Draws a passed texture containing the contents of the layer(s).
		void drawLayers(GLuint layersTexture, std::function<void()> drawArrays);

		// Draw a blue rectangle where there is currently a selection.
		void drawSelectionOverlay(std::function<void()> drawArrays);

    private:
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		void wheelEvent(QGraphicsSceneWheelEvent* wheelEvent);

		Application* app_;

        std::deque<DrawCallback> drawRequests_;
		GlFunctions gl_;
		float zoomPercentage_;

		QOpenGLShaderProgram chequerBoardShader_;
        QOpenGLShaderProgram textureShader_;
		QOpenGLShaderProgram selectionShader_;
		QRectF lastViewportRectangle_;
		QTimer liveUpdateTimer_;

		std::unique_ptr<Texture> canvasCache_;
    };
}

#endif // CANVAS_H

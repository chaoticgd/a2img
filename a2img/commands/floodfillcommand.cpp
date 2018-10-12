#include "floodfillcommand.h"

#include <queue>
#include <cmath>

#include "../application.h"

a2img::FloodFillCommand::FloodFillCommand(Layer* layer, QColor colour, int tolerance)
	: FillCommand(layer, colour, tolerance)
{}

a2img::Texture a2img::FloodFillCommand::renderToTexture(GlFunctions* gl, const Texture& in, bool preview)
{
	in.bind(gl, 0, TextureQuality::pixelated);

	std::vector<Rgba> inputImage(document()->size().width() * document()->size().height());
	std::vector<Rgba> outputImage(document()->size().width() * document()->size().height());

	for(Rgba& e : outputImage) {
		e = Rgba { 0, 0, 0, 255 };
	}

	gl->glGetTexImage(
		GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, inputImage.data()
	);

	// Apply the flood-fill algorithm.
	flood(inputImage, outputImage, document()->size());

	Texture mask(app(), gl, document()->size());
	gl->glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		document()->size().width(), document()->size().height(), 0,
		GL_BGRA, GL_UNSIGNED_BYTE, outputImage.data()
	);

	QColor renderColour = colour();
	if(preview) {
		renderColour.setAlpha(100);
	}
	return app()->maskManager().applyMask(gl, renderColour, in, mask, BlendMode::normal);
}

void a2img::FloodFillCommand::flood(std::vector<Rgba>& inputImage, std::vector<Rgba>& outputImage, QSize size)
{
	auto pixelAt = [=](std::vector<Rgba>& image, QPoint point) {
		return static_cast<Rgba*>(&image[point.y() * size.width() + point.x()]);
	};

	// Compare according to tolerance level.
	auto fuzzyCompare = [=](Rgba a, Rgba b) {
		double distance = std::sqrt((
			std::pow(a.r - b.r, 2) +
			std::pow(a.g - b.g, 2) +
			std::pow(a.b - b.b, 2) +
			std::pow(a.a - b.a, 2)
		) / 4);
		return distance - tolerance() < 0;
	};

	// See https://en.wikipedia.org/wiki/Flood_fill#The_algorithm
	Rgba targetColour =* pixelAt(inputImage, startingPoint());
	std::queue<QPoint> q;
	q.push(startingPoint());
	while(q.size() > 0) {
		QPoint west = q.front();
		q.pop();

		QPoint east = west;

		if(*pixelAt(outputImage, west) == Rgba { 255, 255, 255, 255 }) {
			continue;
		}

		// Find horizontal limits:
		// |         |
		// |<---x--->|
		// |         |
		while(west.x() >= 0 && fuzzyCompare(*pixelAt(inputImage, west), targetColour)) {
			west.setX(west.x() - 1);
		}
		while(east.x() < size.width() && fuzzyCompare(*pixelAt(inputImage, east), targetColour)) {
			east.setX(east.x() + 1);
		}
		// Iterate over the # pixels:
		// |         |
		// |#########|
		// |         |
		for(int x = west.x() + 1; x < east.x(); x++) {
			*pixelAt(outputImage, QPoint(x, west.y())) = Rgba { 255, 255, 255, 255 };
			QPoint north { x, west.y() + 1 };
			QPoint south { x, west.y() - 1 };
			// Inspect upward pixels:
			// |#########|
			// |         |
			// |         |
			if(	  north.y() < size.height()
			   && fuzzyCompare(*pixelAt(inputImage, north), targetColour)
			   &&* pixelAt(outputImage, north) == Rgba { 0, 0, 0, 255 }) {
				q.push(north);
			}
			// Inspect downward pixels:
			// |         |
			// |         |
			// |#########|
			if(	  south.y() >= 0
			   && fuzzyCompare(*pixelAt(inputImage, south), targetColour)
			   &&* pixelAt(outputImage, south) == Rgba { 0, 0, 0, 255 }) {
				q.push(south);
			}
		}
	}
}

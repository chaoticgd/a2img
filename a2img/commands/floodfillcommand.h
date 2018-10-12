#ifndef FLOODFILLCOMMAND_H
#define FLOODFILLCOMMAND_H

#include <QOpenGLFunctions>
#include <QPoint>
#include <QSize>
#include <QColor>

#include "fillcommand.h"

namespace a2img {
	class Application;

	class FloodFillCommand : public FillCommand
	{
	public:
		FloodFillCommand(Layer* layer, QColor colour, int tolerance);

		Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview);

		struct Rgba {
			uint8_t r, g, b, a;

			bool operator==(Rgba const& rhs) {
				return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
			}
		};

		void flood(std::vector<Rgba>& inputImage, std::vector<Rgba>& outputImage, QSize size);
	};
}

#endif // FLOODFILLCOMMAND_H

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

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

#ifndef HSVCOLOURPICKER_H
#define HSVCOLOURPICKER_H

#include <functional>
#include <array>

#include <QColor>
#include <QSlider>

#include "colourpicker.h"
#include "../sliderbox.h"

namespace a2img {
	class Application;

	class HSVColourPicker : public ColourPicker
	{
	public:
		HSVColourPicker(Application* app);

		QString displayName();
		QString iconPath();

		void setColour(QColor newColour);
		void onColourChanged(std::function<void(QColor)> callback);
	private:
		std::array<int, 4> getHsvColour();

		void updateGradientStops();

		Application* app_;

		std::array<SliderBox*, 4> channels_;
		std::function<void(QColor)> callback_;
	};
}

#endif // HSVCOLOURPICKER_H

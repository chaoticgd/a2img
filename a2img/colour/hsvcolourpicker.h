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

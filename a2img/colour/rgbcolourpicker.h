#ifndef RGBCOLOURPICKER_H
#define RGBCOLOURPICKER_H

#include <functional>
#include <array>

#include <QColor>
#include <QSlider>

#include "colourpicker.h"
#include "../sliderbox.h"

namespace a2img {
	class Application;

	class RGBColourPicker : public ColourPicker
	{
	public:
		RGBColourPicker(Application* app);

		QString displayName();
		QString iconPath();

		void setColour(QColor newColour);
		void onColourChanged(std::function<void(QColor)> callback);
	private:
		Application* app_;

		std::array<SliderBox*, 4> channels_;
		std::function<void(QColor)> callback_;
	};
}

#endif // RGBCOLOURPICKER_H

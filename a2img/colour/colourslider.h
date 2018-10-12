#ifndef COLOURSLIDER_H
#define COLOURSLIDER_H

#include <vector>

#include <QColor>
#include <QSlider>

namespace a2img {
	class ColourSlider : public QSlider
	{
	public:
		ColourSlider();
		ColourSlider(std::vector<QColor> gradientStops);

		void setGradientStops(std::vector<QColor> gradientStops);
	};
}

#endif // COLOURSLIDER_H

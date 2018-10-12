#include "colourslider.h"

a2img::ColourSlider::ColourSlider()
	: QSlider(Qt::Horizontal)
{
	setObjectName("colourSlider");
}

a2img::ColourSlider::ColourSlider(std::vector<QColor> gradientStops)
	: QSlider(Qt::Horizontal)
{
	setObjectName("colourSlider");
	setGradientStops(gradientStops);
}

void a2img::ColourSlider::setGradientStops(std::vector<QColor> gradientStops)
{
	QString style = "QSlider::groove { background: qlineargradient(x1:0, y1:0, x2:1, y2:0,";
	for(unsigned int i = 0; i < gradientStops.size(); i++) {
		QColor col = gradientStops[i];
		QString stage = QString::number(static_cast<double>(i) / (gradientStops.size() - 1));
		style += "stop:" + stage + " rgba(" +
			QString::number(col.red()) + ", " +
			QString::number(col.green()) + ", " +
			QString::number(col.blue()) + ", " +
			QString::number(col.alpha()) + "),";
	}
	style += "); }";
	setStyleSheet(style);
}

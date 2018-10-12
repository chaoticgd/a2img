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

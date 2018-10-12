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

#include "hsvcolourpicker.h"

#include <QSlider>
#include <QLabel>
#include <QFormLayout>

#include "../application.h"
#include "colourslider.h"

a2img::HSVColourPicker::HSVColourPicker(Application* app)
	: app_(app)
{
	QFormLayout* layout = new QFormLayout;
	setLayout(layout);

	// Called when any of the channels change value.
	auto onValueChanged = [=](int) {
		if(callback_ == nullptr) {
			return;
		}

		auto colour = getHsvColour();
		callback_(QColor::fromHsv(colour[0], colour[1], colour[2], colour[3]));
		updateGradientStops();
	};

	// Setup colour channel inputs.

	const std::array<int, 4> upperBounds { 360, 255, 255, 255 };
	const std::array<int, 4> values { 0, 0, 0, 255 };
	const std::array<char, 4> labels { 'H', 'S', 'V', 'A' };
	for(size_t i = 0; i < 4; i++) {
		channels_[i] = new SliderBox(new ColourSlider, new QSpinBox);
		channels_[i]->setRange(0, upperBounds[i], 0, upperBounds[i]);
		channels_[i]->setValue(values[i]);
		layout->addRow(QString(labels[i]), channels_[i]);
		QObject::connect(channels_[i]->spinBox(), static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), onValueChanged);
	}

	updateGradientStops();

	// Add the degrees symbol.
	channels_[0]->spinBox()->setSuffix("\u00B0");
}

QString a2img::HSVColourPicker::displayName()
{
	return "HSV";
}

QString a2img::HSVColourPicker::iconPath()
{
	return ":/icons/colourPickerHsv.svg";
}

void a2img::HSVColourPicker::setColour(QColor newColour)
{
	channels_[0]->setValue(newColour.hue());
	channels_[1]->setValue(newColour.saturation());
	channels_[2]->setValue(newColour.value());
	channels_[3]->setValue(newColour.alpha());

	updateGradientStops();
}

void a2img::HSVColourPicker::onColourChanged(std::function<void(QColor)> callback)
{
	callback_ = callback;
}

std::array<int, 4> a2img::HSVColourPicker::getHsvColour()
{
	return {
		channels_[0]->spinBox()->value() % 360,
		channels_[1]->spinBox()->value(),
		channels_[2]->spinBox()->value(),
		channels_[3]->spinBox()->value()
	};
}

void a2img::HSVColourPicker::updateGradientStops()
{
	auto hsvColour = getHsvColour();
	std::vector<std::vector<QColor>> gradientStops {
		// Hue
		{
			{ 255, 0,   0 },
			{ 255, 255, 0 },
			{ 0,   255, 0 },
			{ 0,   255, 255 },
			{ 0,   0,   255 },
			{ 255, 0,   255 },
			{ 255, 0,   0 }
		},
		// Saturation
		{
			QColor::fromHsv(hsvColour[0], 0, hsvColour[2], 255),
			QColor::fromHsv(hsvColour[0], 255, hsvColour[2], 255)
		},
		// Value
		{
			QColor::fromHsv(hsvColour[0], hsvColour[1], 0, 255),
			QColor::fromHsv(hsvColour[0], hsvColour[1], 255, 255)
		},
		// Alpha
		{ { 0, 0, 0 }, { 255, 255, 255 } }
	};
	for(size_t i = 0; i < 4; i++) {
		ColourSlider* slider = static_cast<ColourSlider*>(channels_[i]->slider());
		slider->setGradientStops(gradientStops[i]);
	}
}

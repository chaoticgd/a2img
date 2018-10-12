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

#include "sliderbox.h"

a2img::SliderBox::SliderBox(QSlider* slider, QSpinBox* spinBox)
{
	setLayout(new QHBoxLayout);
	layout()->setMargin(0);

	slider_ = slider;
	layout()->addWidget(slider_);

	spinBox_ = spinBox;
	layout()->addWidget(spinBox_);

	// Link the slider and spin box together.
	QObject::connect(slider_, &QSlider::valueChanged,
					 spinBox_, &QSpinBox::setValue);
	QObject::connect(spinBox_, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
					 slider_, &QSlider::setValue);
}

void a2img::SliderBox::setValue(int value)
{
	slider_->setValue(value);
	slider_->setValue(value);
}

void a2img::SliderBox::setRange(int lower, int upper, int hardLower, int hardUpper)
{
	slider_->setRange(lower, upper);
	spinBox_->setRange(hardLower, hardUpper);
}

a2img::SliderBoxF::SliderBoxF(QSlider* slider, QDoubleSpinBox* spinBox, double sliderScale)
	: sliderScale_(sliderScale),
	  locked_(false)
{
	setLayout(new QHBoxLayout);
	layout()->setMargin(0);

	slider_ = slider;
	layout()->addWidget(slider_);

	spinBox_ = spinBox;
	layout()->addWidget(spinBox_);

	// Link the slider and spin box together.
	QObject::connect(
		slider_,
		&QSlider::valueChanged,
		[=](int newValue) {
			if(!locked_) {
				locked_ = true;
				spinBox_->setValue(newValue / sliderScale_);
				locked_ = false;
			}
		}
	);
	QObject::connect(
		spinBox_,
		static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		[=](double newValue) {
			if(!locked_) {
				locked_ = true;
				slider_->setValue(static_cast<int>(newValue * sliderScale_));
				locked_ = false;
			}
		}
	);
}

void a2img::SliderBoxF::setValue(double value)
{
	slider_->setValue(static_cast<int>(value * sliderScale_));
	spinBox_->setValue(value);
}

void a2img::SliderBoxF::setRange(double lower, double upper, double hardLower, double hardUpper)
{
	slider_->setRange(
		static_cast<int>(lower * sliderScale_),
		static_cast<int>(upper * sliderScale_)
	);
	spinBox_->setRange(hardLower, hardUpper);
}

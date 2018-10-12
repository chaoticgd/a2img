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

#ifndef SLIDERBOX_H
#define SLIDERBOX_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QHBoxLayout>

namespace a2img {
	// A linked QSpinBox and QSlider pair that accepts integer values.
	class SliderBox : public QWidget
	{
	public:
		SliderBox(QSlider* slider, QSpinBox* spinBox);

		QSlider* slider() { return slider_; }
		QSpinBox* spinBox() { return spinBox_; }

		void setValue(int value);
		void setRange(int lower, int upper, int hardLower, int hardUpper);
	private:
		QSlider* slider_;
		QSpinBox* spinBox_;
	};

	// A linked QDoubleSpinBox and QSlider pair that accepts floating point values.
	class SliderBoxF : public QWidget
	{
	public:
		SliderBoxF(QSlider* slider, QDoubleSpinBox* spinBox, double sliderScale);

		QSlider* slider() { return slider_; }
		QDoubleSpinBox* spinBox() { return spinBox_; }

		void setValue(double value);
		void setRange(double lower, double upper, double hardLower, double hardUpper);
	private:
		QSlider* slider_;
		QDoubleSpinBox* spinBox_;
		double sliderScale_;
		bool locked_;
	};
}

#endif // SLIDERBOX_H

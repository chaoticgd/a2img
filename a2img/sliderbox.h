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

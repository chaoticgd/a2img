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

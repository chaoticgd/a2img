#include "rgbcolourpicker.h"

#include <QSlider>
#include <QLabel>
#include <QFormLayout>

#include "../application.h"
#include "colourslider.h"

a2img::RGBColourPicker::RGBColourPicker(Application* app)
	: app_(app)
{
	QFormLayout* layout = new QFormLayout;
	setLayout(layout);

	// Called when any of the channels change value.
	auto onValueChanged = [=](int) {
		QColor newColour {
			channels_[0]->spinBox()->value(),
			channels_[1]->spinBox()->value(),
			channels_[2]->spinBox()->value(),
			channels_[3]->spinBox()->value()
		};

		if(callback_ != 0) {
			callback_(newColour);
		}
	};

	// Setup colour channel inputs.

	const std::vector<std::vector<QColor>> gradientStops {
		{ QColor(0, 0, 0), QColor(255, 0, 0) },
		{ QColor(0, 0, 0), QColor(0, 255, 0) },
		{ QColor(0, 0, 0), QColor(0, 0, 255) },
		{ QColor(0, 0, 0), QColor(255, 255, 255) }
	};
	const std::array<int, 4> values { 0, 0, 0, 255 };
	const std::array<char, 4> labels { 'R', 'G', 'B', 'A' };
	for(int i = 0; i < 4; i++) {
		channels_[i] = new SliderBox(new ColourSlider(gradientStops[i]), new QSpinBox);
		channels_[i]->setRange(0, 255, 0, 255);
		channels_[i]->setValue(values[i]);
		layout->addRow(QString(labels[i]), channels_[i]);
		QObject::connect(channels_[i]->spinBox(), static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), onValueChanged);
	}
}

QString a2img::RGBColourPicker::displayName()
{
	return "RGB";
}

QString a2img::RGBColourPicker::iconPath()
{
	return ":/icons/colourPickerRgb.svg";
}

void a2img::RGBColourPicker::setColour(QColor newColour)
{
	channels_[0]->setValue(newColour.red());
	channels_[1]->setValue(newColour.green());
	channels_[2]->setValue(newColour.blue());
	channels_[3]->setValue(newColour.alpha());
}

void a2img::RGBColourPicker::onColourChanged(std::function<void(QColor)> callback)
{
	callback_ = callback;
}

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

#include "effectproperty.h"

#include <cmath>
#include <QComboBox>
#include <QJsonArray>

#include "application.h"
#include "sliderbox.h"

QString a2img::EffectProperty::glslName()
{
	return glslName_;
}

QString a2img::EffectProperty::displayName()
{
	return displayName_;
}

a2img::EffectProperty::EffectProperty(Application* app, QJsonObject propertyObject)
	: app_(app)
{

	QJsonValue glslNameVal = propertyObject["glslName"];
	if(!glslNameVal.isString()) {
		throw InvalidEffectException("Failed to load effect property: Invalid GLSL name.");
	}
	glslName_ = glslNameVal.toString();

	QJsonValue displayNameVal = propertyObject["displayName"];
	if(!displayNameVal.isString()) {
		throw InvalidEffectException("Failed to load effect property: Invalid display name.");
	}
	displayName_ = displayNameVal.toString();
}

QString a2img::EffectProperty::glslIdentifier()
{
	return "a2_property_" + glslName_;
}

//
// Int
//

a2img::EffectPropertyInt::EffectPropertyInt(Application* app, QJsonObject propertyObject)
	: EffectProperty(app, propertyObject)
{
	// Mandatory Fields

	QJsonValue valueVal = propertyObject["value"];
	if(!valueVal.isDouble()) {
		throw InvalidEffectException("Failed to load effect property: Value not specified.");
	}
	value_ = valueVal.toInt();

	// Optional Fields

	QJsonValue minimumVal = propertyObject["minimum"];
	minimum_ = minimumVal.toInt(0);
	QJsonValue maximumVal = propertyObject["maximum"];
	maximum_ = maximumVal.toInt(999);

	QJsonValue hardMinimumVal = propertyObject["hardMinimum"];
	hardMinimum_ = hardMinimumVal.toInt(minimum_);
	QJsonValue hardMaximumVal = propertyObject["hardMaximum"];
	hardMaximum_ = hardMaximumVal.toInt(maximum_);

	QJsonValue prefixVal = propertyObject["prefix"];
	prefix_ = prefixVal.toString("");
	QJsonValue suffixVal = propertyObject["suffix"];
	suffix_ = suffixVal.toString("");
}

std::unique_ptr<a2img::EffectProperty> a2img::EffectPropertyInt::copy()
{
	return std::unique_ptr<EffectProperty>(new EffectPropertyInt(*this));
}

QWidget* a2img::EffectPropertyInt::getInputWidget()
{
	SliderBox* input = new SliderBox(new QSlider(Qt::Horizontal), new QSpinBox);

	input->setRange(minimum_, maximum_, hardMinimum_, hardMaximum_);
	input->setValue(value_);
	input->spinBox()->setPrefix(prefix_);
	input->spinBox()->setSuffix(suffix_);

	QObject::connect(
		input->spinBox(),
		static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		[=](int newValue) {
			value_ = newValue;
			app_->canvas()->update(RedrawMode::fullRedraw);
		}
	);

	return input;
}

void a2img::EffectPropertyInt::attach(GlFunctions* gl, QOpenGLShaderProgram* shader)
{
	GLuint location = shader->uniformLocation(glslIdentifier());
	gl->glUniform1i(location, value_);
}

//
// Float
//

a2img::EffectPropertyFloat::EffectPropertyFloat(Application* app, QJsonObject propertyObject)
	: EffectProperty(app, propertyObject)
{
	// Mandatory Fields

	QJsonValue valueVal = propertyObject["value"];
	if(!valueVal.isDouble()) {
		throw InvalidEffectException("Failed to load effect property: Value not specified.");
	}
	value_ = valueVal.toDouble();

	// Optional Fields

	QJsonValue minimumVal = propertyObject["minimum"];
	minimum_ = minimumVal.toDouble(0);
	QJsonValue maximumVal = propertyObject["maximum"];
	maximum_ = maximumVal.toDouble(999);

	QJsonValue hardMinimumVal = propertyObject["hardMinimum"];
	hardMinimum_ = hardMinimumVal.toDouble(minimum_);
	QJsonValue hardMaximumVal = propertyObject["hardMaximum"];
	hardMaximum_ = hardMaximumVal.toDouble(maximum_);

	QJsonValue prefixVal = propertyObject["prefix"];
	prefix_ = prefixVal.toString("");
	QJsonValue suffixVal = propertyObject["suffix"];
	suffix_ = suffixVal.toString("");

	QJsonValue sfVal = propertyObject["decimalPlaces"];
	decimalPlaces_ = sfVal.toInt(0);
}

std::unique_ptr<a2img::EffectProperty> a2img::EffectPropertyFloat::copy()
{
	return std::unique_ptr<EffectProperty>(new EffectPropertyFloat(*this));
}

QWidget* a2img::EffectPropertyFloat::getInputWidget()
{
	int sliderScale = std::pow(10, decimalPlaces_);
	SliderBoxF* input = new SliderBoxF(new QSlider(Qt::Horizontal), new QDoubleSpinBox, sliderScale);

	input->setRange(minimum_, maximum_, hardMinimum_, hardMaximum_);
	input->setValue(value_);
	input->spinBox()->setPrefix(prefix_);
	input->spinBox()->setSuffix(suffix_);
	input->spinBox()->setDecimals(decimalPlaces_);

	QObject::connect(
		input->spinBox(),
		static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		[=](double newValue) {
			value_ = newValue;
			app_->canvas()->update(RedrawMode::fullRedraw);
		}
	);

	return input;
}

void a2img::EffectPropertyFloat::attach(GlFunctions* gl, QOpenGLShaderProgram* shader)
{
	GLuint location = shader->uniformLocation(glslIdentifier());
	gl->glUniform1f(location, value_);
}

//
// Enum
//

a2img::EffectPropertyEnum::EffectPropertyEnum(Application* app, QJsonObject propertyObject)
	: EffectProperty(app, propertyObject)
{
	// Mandatory Fields

	QJsonValue value = propertyObject["value"];
	value_ = value.toInt();

	QJsonValue options = propertyObject["options"];
	for(QJsonValue item : options.toArray()) {
		options_.push_back(item.toString());
	}
}

std::unique_ptr<a2img::EffectProperty> a2img::EffectPropertyEnum::copy()
{
	return std::unique_ptr<EffectProperty>(new EffectPropertyEnum(*this));
}

QWidget* a2img::EffectPropertyEnum::getInputWidget()
{
	QComboBox* input = new QComboBox;

	for(QString option : options_) {
		input->addItem(option);
	}

	return input;
}

void a2img::EffectPropertyEnum::attach(GlFunctions* gl, QOpenGLShaderProgram* shader)
{
	GLuint location = shader->uniformLocation(glslIdentifier());
	gl->glUniform1i(location, value_);
}

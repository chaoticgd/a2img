#ifndef EFFECTPROPERTY_H
#define EFFECTPROPERTY_H

#include <memory>
#include <vector>

#include <QWidget>
#include <QJsonObject>
#include <QOpenGLShaderProgram>

#include "glfunctions.h"

namespace a2img {
	class Application;
	class EffectPropertyInt;

	// A property of an effect that is modifiable by the user.
	// Properties can be of different types, each of which inherit from the
	// EffectProperty base class.
	class EffectProperty
	{
	public:
		virtual ~EffectProperty() {}

		QString glslName();
		QString displayName();

		virtual std::unique_ptr<EffectProperty> copy() = 0;
		// Returns a new widget that can be used by the user to
		// modify the effect.
		virtual QWidget* getInputWidget() = 0;
		virtual void attach(GlFunctions* gl, QOpenGLShaderProgram* shader) = 0;

	protected:
		EffectProperty(Application* app) : app_(app) {}
		EffectProperty(Application* app, QJsonObject propertyObject);

		QString glslIdentifier();

		Application* app_;

		QString glslName_;
		QString displayName_;
	};

	// A property that is a whole number (e.g. number of Mandelbrot iterations).
	class EffectPropertyInt : public EffectProperty
	{
	public:
		EffectPropertyInt(Application* app, QJsonObject propertyObject);

		std::unique_ptr<EffectProperty> copy();
		QWidget* getInputWidget();
		void attach(GlFunctions* gl, QOpenGLShaderProgram* shader);

	private:
		EffectPropertyInt(Application* app) : EffectProperty(app) {}

		int value_;
		int minimum_;
		int maximum_;
		int hardMinimum_;
		int hardMaximum_;
		QString prefix_;
		QString suffix_;
	};

	// A proprty that is a floating point number.
	class EffectPropertyFloat : public EffectProperty
	{
	public:
		EffectPropertyFloat(Application* app, QJsonObject propertyObject);

		std::unique_ptr<EffectProperty> copy();
		QWidget* getInputWidget();
		void attach(GlFunctions* gl, QOpenGLShaderProgram* shader);

	private:
		EffectPropertyFloat(Application* app) : EffectProperty(app) {}

		float value_;
		float minimum_;
		float maximum_;
		float hardMinimum_;
		float hardMaximum_;
		QString prefix_;
		QString suffix_;
		int decimalPlaces_;
	};

	class EffectPropertyEnum : public EffectProperty {
	public:
		EffectPropertyEnum(Application* app, QJsonObject propertyObject);

		std::unique_ptr<EffectProperty> copy();
		QWidget* getInputWidget();
		void attach(GlFunctions* gl, QOpenGLShaderProgram* shader);

	private:
		int value_;
		std::vector<QString> options_;
	};
}

#endif // EFFECTPROPERTY_H

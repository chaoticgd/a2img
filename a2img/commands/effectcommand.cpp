#include "effectcommand.h"

#include <algorithm>
#include <QFormLayout>

#include "vectorutil.h"
#include "application.h"

a2img::EffectCommand::EffectCommand(Layer* layerArgument, QString category, QString name)
	: TextureStorageCommand(layerArgument),
	  package_(category),
	  name_(name)
{
	setText(effect()->displayName());

	std::vector<EffectProperty*> props = effect()->properties();
	properties_.resize(props.size());
	std::transform(
		props.begin(),
		props.end(),
		properties_.begin(),
		[](EffectProperty* e) {
			return std::unique_ptr<EffectProperty>(e->copy());
		}
	);
}

a2img::Texture a2img::EffectCommand::renderToTexture(GlFunctions* gl, const Texture& in, bool preview)
{
	// Draw the whole image with the effect applied.
	GLuint output = effect()->draw(gl, in, document()->size(), properties());
	return Texture(app(), output, in.size());
}

void a2img::EffectCommand::populateInputPanel(QWidget* outer)
{
	QFormLayout* layout = new QFormLayout;
	outer->setLayout(layout);

	for(EffectProperty* property : properties()) {
		layout->addRow(property->displayName(), property->getInputWidget());
	}
}

QString a2img::EffectCommand::type() const
{
	return "applyEffect";
}

QString a2img::EffectCommand::package() const
{
	return package_;
}

QString a2img::EffectCommand::name() const
{
	return name_;
}

std::vector<a2img::EffectProperty*> a2img::EffectCommand::properties()
{
	return uniqueVectorToRaw<EffectProperty>(properties_);
}

void a2img::EffectCommand::validateProperties()
{
	Effect* e = effect();

	// Check for new properties to be added, and [1].
	for(EffectProperty* templateProperty : e->properties()) {
		bool propertyExists = false;
		for(EffectProperty* instanceProperty : properties()) {
			if(templateProperty->glslName() == instanceProperty->glslName()) {
				//* *** [1] TODO: Update properties. ****
				// instanceProperty->updateFrom(templateProperty);
				propertyExists = true;
				break;
			}
		}

		if(!propertyExists) {
			properties_.emplace_back(templateProperty->copy());
		}
	}

	// Check for properties that no longer exist, to be removed.
	for(auto instanceProperty = properties_.begin(); instanceProperty < properties_.end(); instanceProperty++) {
		bool templateExists = false;
		for(EffectProperty* templateProperty : e->properties()) {
			if((*instanceProperty)->glslName() == templateProperty->glslName()) {
				templateExists = true;
			}
		}

		if(!templateExists) {
			properties_.erase(instanceProperty);
		}
	}
}

a2img::Effect* a2img::EffectCommand::effect()
{
	return app()->effectManager().effect(package_, name_);
}

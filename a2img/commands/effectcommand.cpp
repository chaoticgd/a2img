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

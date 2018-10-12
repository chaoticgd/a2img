#ifndef EFFECTCOMMAND_H
#define EFFECTCOMMAND_H

#include <string>

#include "texturestoragecommand.h"
#include "../effect.h"

namespace a2img {
	class EffectCommand : public TextureStorageCommand
	{
	public:
		EffectCommand(Layer* layerArgument, QString package, QString name);

		Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview);
		void populateInputPanel(QWidget* outer);

		QString type() const;

		QString package() const;
		QString name() const;

		std::vector<EffectProperty*> properties();
		void validateProperties();

	private:
		Effect* effect();

		QString package_;
		QString name_;

		std::vector<std::unique_ptr<EffectProperty>> properties_;
	};
}

#endif // EFFECTCOMMAND_H

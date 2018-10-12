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

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

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include <memory>

#include <QScrollArea>

#include "effect.h"
#include "commands/texturestoragecommand.h"

namespace a2img {
	class Application;
	class EffectCommand;

	class EffectManager
	{
	public:
		EffectManager(Application*);

		std::vector<Effect*> effects();
		Effect* effect(QString package, QString name);

		QScrollArea* effectInput() { return effectInput_; }

		void reload();

		void updatePanel();
	private:
		void loadPanel();
		void applyEffects();

		Application* app_;

		std::vector<std::unique_ptr<Effect>> effects_;

		QScrollArea* effectInput_;
		TextureStorageCommand* selectedQueuedCommand_;
	};
}

#endif // EFFECTMANAGER_H

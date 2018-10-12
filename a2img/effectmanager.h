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

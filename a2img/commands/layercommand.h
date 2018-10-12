#ifndef LAYERCOMMAND_H
#define LAYERCOMMAND_H

#include "serialisablecommand.h"

namespace a2img {
	class Application;
	class Document;
	class Layer;

	typedef qint64 LayerId;

	// A command that affects a specific layer.
	class LayerCommand : public SerialisableCommand
	{
	protected:
		LayerCommand(Layer* layer);

		virtual void serialise(DocumentWriter& storage, QJsonObject& metadata) const = 0;

		Application* app();

	public:
		Document* document();
		Layer* layer();

	private:
		Application* app_;
		Document* document_;
		Layer* layer_;
	};
}

#endif // LAYERCOMMAND_H

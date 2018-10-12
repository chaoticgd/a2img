#ifndef TEXTURESTORAGECOMMAND_H
#define TEXTURESTORAGECOMMAND_H

#include <memory>

#include "layercommand.h"
#include "../glfunctions.h"
#include "../texture.h"

namespace a2img {
	class Application;
	class Layer;
	class Document;

	// Stores a beforeTexture_ which will be
	// used by subclasses to store the state of
	// a layer before a command was applied.
	//
	// If the user chooses to undo said
	// command, the undo() function will
	// automatically revert the state of
	// the layer based on the contents
	// of beforeTexture_.
	//
	// Also handles storing, and provides
	// functions for applying selection masks.
	class TextureStorageCommand : public LayerCommand
	{
	public:
		TextureStorageCommand(Layer* layer);

		// Replaces the layer's texture with the result of
		// renderToTexture and updates the beforeTexture_.
		void redo();

		// Reverts the state of the layer
		// to the beforeTexture_.
		void undo();

		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

		// Called when the command is pushed onto the
		// document history stack.
		void onApplied();

		virtual Texture renderToTexture(GlFunctions* gl, const Texture& in, bool preview) = 0;
		virtual void populateInputPanel(QWidget* outer) = 0;

	private:
		bool isSelecting_;
		Texture beforeTexture_;
		std::shared_ptr<Texture> selectionMask_;
	};
}

#endif // TEXTURESTORAGECOMMAND_H

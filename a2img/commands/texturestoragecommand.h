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

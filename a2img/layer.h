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

#ifndef LAYER_H
#define LAYER_H

#include <functional>
#include <memory>

#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QSize>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "glfunctions.h"
#include "commands/effectcommand.h"
#include "texture.h"
#include "treenode.h"

namespace a2img {
	class Application;
	class Document;

	// Unique identifier for a layer in a document,
	// which is persistent across sessions.
	typedef qint64 LayerId;

	class Layer : public TreeNode<Layer>
	{
	public:
		Layer(Application* app, Document* document, QString displayName);

		// Initialise the underlying texture to be a solid colour.
		void create(QSize size, QColor backgroundColour);

		// Initialise the underlying texture from an image
		void create(QImage image);

		Application* app();
		Document* document();

		QString displayName();
		void setDisplayName(QString displayName);

		LayerId id();
		QSize size();

		// Returns the current underlying texture ID.
		// Does not transfer ownership or replace the original.
		GLuint texture();

		// Replace the current underlying texture ID with the one
		// returned by the callback.
		void replaceTexture(QUndoCommand*, std::function<Texture(Texture& in)> callback);

		// Returns a list of all texture maps used by this layer.
		// e.g. Colour Map
		std::vector<Texture*> maps();

		// Draw the layer to the framebuffer object passed as the
		// second argument. Requires a valid OpenGL context.
		void draw(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview);

		// Represents an invalid layer ID (value of 0).
		static LayerId invalidId;
	private:
		void drawSelf(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview);
		void drawChildren(GlFunctions* gl, QOpenGLFramebufferObject& fbo, bool preview);
	public:

		void addQueuedCommand(std::unique_ptr<TextureStorageCommand> effect);
		void removeQueuedCommand(TextureStorageCommand* effect);
		void moveQueuedCommand(TextureStorageCommand* effect, bool towardsEnd);
		std::vector<TextureStorageCommand*> queuedCommands();

		// Apply all effects to the underlying texture
		// and clear pendingEffects_.
		void applyEffects();

		// When a layer is deleted via a DeleteLayerCommand object,
		// its deleted state is set to true. If it is later restored,
		// its deleted state is set to false.
		bool isDeleted();
		void setDeleted(bool isDeleted);

		bool isVisible();
		void setVisible(bool isVisible);

		QPixmap thumbnail();
		void updateThumbnail();

		// Serialise metadata for this layer and all child layers.
		QJsonObject serialiseMetadata();

	private:
		Application* app_;
		Document* document_;

		QString name_;
		Texture texture_;
		std::vector<std::unique_ptr<TextureStorageCommand>> queuedCommands_;
		bool isVisible_;
		QPixmap thumbnail_;
	};
}

#endif // LAYER_H

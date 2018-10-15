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

#ifndef IMAGERESIZECOMMAND_H
#define IMAGERESIZECOMMAND_H

#include <QOpenGLShaderProgram>

#include "layercommand.h"
#include "util/vec2.h"

namespace a2img {
    class ImageResizeCommand : public SerialisableCommand
	{
	public:
		ImageResizeCommand(Application* app, Document* document, int newWidth, int newHeight, bool stretch, bool smooth);

		void redo();
		void undo();

		QString type() const;
		void serialise(DocumentWriter& storage, QJsonObject& metadata) const;

	private:
		Application* app_;
		Document* document_;
		Vec2<int> newSize_;
		bool stretch_, smooth_;

		Vec2<int> oldSize_;
		std::vector<Texture> oldTextures_;
	};
}

#endif // IMAGERESIZECOMMAND_H

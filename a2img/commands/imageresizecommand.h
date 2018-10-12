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

		QOpenGLShaderProgram textureShader_;
	};
}

#endif // IMAGERESIZECOMMAND_H

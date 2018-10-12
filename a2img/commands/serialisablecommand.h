#ifndef SERIALISABLECOMMAND_H
#define SERIALISABLECOMMAND_H

#include <QUndoCommand>

#include "document/documentwriter.h"

namespace a2img {
	class SerialisableCommand : public QUndoCommand {
	public:
		virtual QString type() const = 0;
		virtual void serialise(DocumentWriter& storage, QJsonObject& metadata) const = 0;
	};
}

#endif // SERIALISABLECOMMAND_H

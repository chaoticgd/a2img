#ifndef SERIALISATION_H
#define SERIALISATION_H

#include <QString>
#include <QJsonObject>
#include <QPointF>
#include <QColor>
#include <QSize>

#include "util/vec2.h"

namespace a2img {
	namespace serialisation {
		// toString

		template <typename T>
		QString toString(Vec2<T> in);

		// toJsonObject

		template <typename T>
		QJsonObject toJsonObject(Vec2<T> in);
		QJsonObject toJsonObject(QPointF in);
		QJsonObject toJsonObject(QColor in);
		QJsonObject toJsonObject(QSize in);

		// fromJsonObject

	}
}


template <typename T>
QJsonObject a2img::serialisation::toJsonObject(Vec2<T> in)
{
	QJsonObject result;
	result.insert("x", in.x);
	result.insert("y", in.y);
	return result;
}

template <typename T>
QString a2img::serialisation::toString(Vec2<T> in)
{
	return "(" + QString::number(in.x) + ", " + QString::number(in.y) + ")";
}

#endif // SERIALISATION_H

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

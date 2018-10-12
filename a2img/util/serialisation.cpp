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

#include "serialisation.h"

QJsonObject a2img::serialisation::toJsonObject(QPointF in)
{
	QJsonObject result;
	result.insert("x", in.x());
	result.insert("y", in.y());
	return result;
}

QJsonObject a2img::serialisation::toJsonObject(QColor in)
{
	QJsonObject result;
	result.insert("r", in.red());
	result.insert("g", in.green());
	result.insert("b", in.blue());
	result.insert("a", in.alpha());
	return result;
}

QJsonObject a2img::serialisation::toJsonObject(QSize in)
{
	QJsonObject result;
	result.insert("width", in.width());
	result.insert("height", in.height());
	return result;
}

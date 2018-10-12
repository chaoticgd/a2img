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

#include "fillcommand.h"

#include <QJsonObject>

#include "util/serialisation.h"

a2img::FillCommand::FillCommand(Layer* layer, QColor colour, int tolerance)
	: TextureStorageCommand(layer),
	  colour_(colour),
	  tolerance_(tolerance)
{}

QString a2img::FillCommand::type() const
{
	return "fill";
}

void a2img::FillCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject json;
	json.insert("startingPoint", serialisation::toJsonObject(startingPoint_));
	json.insert("colour", serialisation::toJsonObject(colour_));
	json.insert("tolerance", tolerance_);
	metadata.insert("fill", json);
}

void a2img::FillCommand::populateInputPanel(QWidget* outer)
{

}

void a2img::FillCommand::setStartingPoint(QPointF point)
{
	startingPoint_.setX(point.x());
	startingPoint_.setY(point.y());
}

QPoint a2img::FillCommand::startingPoint()
{
	return startingPoint_;
}

QColor a2img::FillCommand::colour()
{
	return colour_;
}

int a2img::FillCommand::tolerance()
{
	return tolerance_;
}

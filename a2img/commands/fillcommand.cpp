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

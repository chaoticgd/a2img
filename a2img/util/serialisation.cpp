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

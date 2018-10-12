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

#include "brushcommand.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <math.h>

#include <QVector2D>
#include <QPainter>

#include "../application.h"
#include "../document/document.h"
#include "../util/rectangle.h"
#include "../vectorutil.h"
#include "../util/serialisation.h"

#define M_PI 3.14159265358979323846

a2img::BrushCommand::BrushCommand(Layer* layer, int diameter, QColor colour, BlendMode blendMode)
	: TextureStorageCommand(layer),
	  diameter_(diameter),
	  colour_(colour),
	  blendMode_(blendMode)
{
	setText("Brush Stroke");
}

QString a2img::BrushCommand::type() const
{
	return "brushStroke";
}

void a2img::BrushCommand::serialise(DocumentWriter& storage, QJsonObject& metadata) const
{
	QJsonObject json;

	QJsonArray pointsJson;
	for(QPointF point : points_) {
		pointsJson.append(serialisation::toJsonObject(point));
	}
	json.insert("points", pointsJson);
	json.insert("diameter", diameter_);
	json.insert("colour", serialisation::toJsonObject(colour_));
	json.insert("blendMode", blendMode_ == BlendMode::multiply ? "multiply" : "normal");
	metadata.insert("brush", json);

	TextureStorageCommand::serialise(storage, metadata);
}

a2img::Texture a2img::BrushCommand::renderToTexture(GlFunctions* gl, const Texture& in, bool preview)
{
	A2_GL_ENTER(gl)

	// Draw white-on-black mask.
	QImage maskImage(document()->size().width(), document()->size().height(), QImage::Format_RGB32);
	maskImage.fill(QColor(0, 0, 0, 0));
	QPainter maskPainter(&maskImage);
	drawPainter(&maskPainter);
	maskPainter.end();

	// Draw final image.
	const Texture mask(app(), gl, maskImage);

	A2_GL_EXIT(gl)

	return app()->maskManager().applyMask(gl, colour_, in, mask, blendMode_);
}

void a2img::BrushCommand::populateInputPanel(QWidget* outer)
{

}

void a2img::BrushCommand::appendPoint(QPointF newPoint)
{
	points_.push_back(newPoint);
}

void a2img::BrushCommand::drawPainter(QPainter* painter)
{
	QSize docSize = document()->size();

	// Specify width of brush, colour and shape.
	QPen pen;
	pen.setWidth(diameter_);
	pen.setColor(QColor(255, 255, 255));
	pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);

	if(points_.size() == 1) {
		painter->drawLine(QLine {
			static_cast<int>(points_[0].x()),
			static_cast<int>(docSize.height() - points_[0].y()),
			static_cast<int>(points_[0].x()),
			static_cast<int>(docSize.height() - points_[0].y())
		});
	}

	// Iterate over every pair of points, drawing lines between them.
	for(auto iter = points_.begin(); iter < points_.end() - 1 && iter < points_.end(); iter++) {
		QPointF last = *iter;
		QPointF current = *(iter + 1);

		QLine line {
			static_cast<int>(last.x()),
			static_cast<int>(docSize.height() - last.y()),
			static_cast<int>(current.x()),
			static_cast<int>(docSize.height() - current.y())
		};

		painter->drawLine(line);
	}
}

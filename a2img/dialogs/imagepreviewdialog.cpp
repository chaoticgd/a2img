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

#include "imagepreviewdialog.h"

#include <QLabel>
#include <QLayout>

a2img::ImagePreviewDialog::ImagePreviewDialog(QWidget* parent)
	: QFileDialog(parent)
{
	QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());

	QLabel* previewImageWidget = new QLabel;
	previewImageWidget->setMinimumSize(100, 100);
	gridLayout->addWidget(previewImageWidget, 0, 3, 2, 1);

	connect(this, &QFileDialog::currentChanged, [=](const QString& path) {
		QImage image(path);
		if(image.isNull()) {
			previewImageWidget->setPixmap(QPixmap());
		} else {
			previewImageWidget->setPixmap(QPixmap::fromImage(image.scaled(100, 100, Qt::KeepAspectRatio)));
		}
	});
}

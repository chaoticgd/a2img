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

#include "newimagedialog.h"
#include "ui_newimagedialog.h"

NewImageDialog::NewImageDialog(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::NewImageDialog)
{
	ui->setupUi(this);

	ui->backgroundInput->addItems(QStringList { "Transparent", "White" });
}

NewImageDialog::~NewImageDialog()
{
	delete ui;
}

QSize NewImageDialog::getDocumentSize()
{
	return QSize(ui->widthInput->value(), ui->heightInput->value());
}

QColor NewImageDialog::getDocumentBackgroundColour()
{
	if(ui->backgroundInput->currentText() == "White") {
		return QColor(255, 255, 255, 255);
	}

	return QColor(0, 0, 0, 0);
}

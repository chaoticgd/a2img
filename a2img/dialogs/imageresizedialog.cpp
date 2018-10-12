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

#include "imageresizedialog.h"
#include "ui_imageresizedialog.h"

#include "application.h"
#include "commands/imageresizecommand.h"

ImageResizeDialog::ImageResizeDialog(a2img::Application* app, QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::ResizeImageDialog),
    app_(app)
{
    ui_->setupUi(this);
}

ImageResizeDialog::~ImageResizeDialog()
{
    delete ui_;
}

void ImageResizeDialog::on_buttonBox_accepted()
{
	a2img::Document* doc = app_->documentManager().currentDocument();
    int x = ui_->widthBox->value(), y = ui_->heightBox->value();
	bool stretch = ui_->modeStretch->isChecked();
	bool smooth = ui_->qualityInterpolated->isChecked();

	a2img::ImageResizeCommand* command = new a2img::ImageResizeCommand(app_, doc, x, y, stretch, smooth);
	doc->pushUndoCommand(command);
}

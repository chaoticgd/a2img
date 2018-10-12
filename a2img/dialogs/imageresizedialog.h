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

#ifndef IMAGERESIZEDIALOG_H
#define IMAGERESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeImageDialog;
}

namespace a2img {
    class Application;
}

class ImageResizeDialog : public QDialog
{
    Q_OBJECT

public:
	explicit ImageResizeDialog(a2img::Application* app, QWidget* parent = nullptr);
	~ImageResizeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ResizeImageDialog* ui_;

    a2img::Application* app_;
};

#endif // IMAGERESIZEDIALOG_H

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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

namespace a2img {
	class Application;
}

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(a2img::Application* app, QWidget* parent = nullptr);
	~AboutDialog();

protected:
	// Replace a template from the 'About' text (e.g. to add the correct build date).
	void replaceTemplate(QString original, QString replacement);

private:
	Ui::AboutDialog* ui_;
};

#endif // ABOUTDIALOG_H

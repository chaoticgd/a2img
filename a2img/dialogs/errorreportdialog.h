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

#ifndef ERRORREPORTDIALOG_H
#define ERRORREPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorReportDialog;
}

class ErrorReportDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ErrorReportDialog(QString message, QWidget* parent = 0);
	~ErrorReportDialog();

private:
	Ui::ErrorReportDialog* ui_;
};

#endif // ERRORREPORTDIALOG_H

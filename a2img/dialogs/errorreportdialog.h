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

#include "errorreportdialog.h"
#include "ui_errorreportdialog.h"

ErrorReportDialog::ErrorReportDialog(QString message, QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::ErrorReportDialog)
{
	ui_->setupUi(this);
	ui_->reportArea->setStyleSheet("font-family: Monospace");
	ui_->reportArea->setPlainText(message);
}

ErrorReportDialog::~ErrorReportDialog()
{
	delete ui_;
}

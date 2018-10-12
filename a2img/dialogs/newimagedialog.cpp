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

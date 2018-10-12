#ifndef NEWIMAGEDIALOG_H
#define NEWIMAGEDIALOG_H

#include <QDialog>
#include <QSize>
#include <QColor>

namespace Ui {
class NewImageDialog;
}

// Displays a 'New Image' dialog allowing the user to adjust the size and initial colour of their image.
class NewImageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewImageDialog(QWidget* parent = 0);
	~NewImageDialog();

	// Get the dimensions that the user selected.
	QSize getDocumentSize();
	// Get the background colour that the user selected.
	QColor getDocumentBackgroundColour();

private:
	Ui::NewImageDialog* ui;
};

#endif // NEWIMAGEDIALOG_H

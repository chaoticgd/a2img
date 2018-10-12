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

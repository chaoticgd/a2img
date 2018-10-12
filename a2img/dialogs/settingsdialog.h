#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "application.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(a2img::Application* app, QWidget* parent = nullptr);
	~SettingsDialog();

	void saveSettings();

private slots:
	void on_buttonBox_accepted();

	void on_buttonBox_clicked(QAbstractButton* button);

private:
	Ui::SettingsDialog* ui_;

	a2img::Application* app_;
};

#endif // SETTINGSDIALOG_H

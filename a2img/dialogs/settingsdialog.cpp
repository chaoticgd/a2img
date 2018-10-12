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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QStyleFactory>

SettingsDialog::SettingsDialog(a2img::Application* app, QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::SettingsDialog),
	app_(app)
{
	ui_->setupUi(this);

	a2img::SettingsManager& config = app_->settingsManager();

	config.beginGroup("userInterface");

	ui_->uiThemeInput->addItem("Default");
	for(auto theme : app_->themeManager().themes()) {
		ui_->uiThemeInput->addItem(theme.first);
	}
	ui_->uiThemeInput->setCurrentText(config.value("theme", "Default").toString());
	ui_->uiToolbarStyleCompact->setChecked(config.value("toolbarStyle", "compact").toString() == "compact");

	config.endGroup();
}

SettingsDialog::~SettingsDialog()
{
	delete ui_;
}


void SettingsDialog::saveSettings()
{
	a2img::SettingsManager& config = app_->settingsManager();

	config.beginGroup("general");
	config.setValue("multiProcess", ui_->generalMpOn->isChecked());
	config.endGroup();

	config.beginGroup("userInterface");
	config.setValue("theme", ui_->uiThemeInput->currentText());
	config.setValue("toolbarStyle", ui_->uiToolbarStyleCompact->isChecked() ? "compact" : "large");
	config.endGroup();
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton* button)
{
	if(button != ui_->buttonBox->button(QDialogButtonBox::Cancel)) {
		saveSettings();
	}
}

void SettingsDialog::on_buttonBox_accepted()
{

}

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

#include "settingsmanager.h"

a2img::SettingsManager::SettingsManager(QString basePath)
	: QSettings(basePath + "/settings.ini", QSettings::IniFormat)
{

}

void a2img::SettingsManager::setValue(const QString& key, const QVariant& value)
{
	QSettings::setValue(key, value);

	std::pair<QString, QString> signature { group(), key };
	if(callbacks_.find(signature) != callbacks_.end()) {
		callbacks_.at(signature)(value);
	}
}

void a2img::SettingsManager::onValueChanged(QString group, QString key, std::function<void(QVariant value)> callback)
{
	std::pair<QString, QString> signature { group, key };
	callbacks_.emplace(signature, callback);
}

void a2img::SettingsManager::update()
{
	for(auto callback : callbacks_) {
		beginGroup(callback.first.first);
		QVariant newValue = value(callback.first.second);
		endGroup();
		callback.second(newValue);
	}
}

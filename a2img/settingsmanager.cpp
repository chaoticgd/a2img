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

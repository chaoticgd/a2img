#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <map>
#include <functional>
#include <QSettings>
#include <QString>

namespace a2img {
	class SettingsManager : public QSettings
	{
	public:
		SettingsManager(QString basePath);

		void setValue(const QString& key, const QVariant& value);
		void onValueChanged(QString group, QString key, std::function<void(QVariant)> callback);
		void update();

	private:
		std::map<std::pair<QString, QString>, std::function<void(QVariant)>> callbacks_;
	};
}

#endif // SETTINGSMANAGER_H

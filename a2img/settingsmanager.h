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

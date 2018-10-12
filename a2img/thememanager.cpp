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

#include "thememanager.h"

#include <QDir>

a2img::ThemeManager::ThemeManager(QString basePath)
	: basePath_(basePath)
{
	reload();
}

void a2img::ThemeManager::reload()
{
	const auto flags = QDir::Dirs | QDir::NoDotAndDotDot;

	QDir themesDir(basePath_ + "/themes");
	QStringList themeNames = themesDir.entryList(flags);
	for(QString themeName : themeNames) {
		QFile styleSheet(basePath_ + "/themes/" + themeName + "/stylesheet.qss");
		styleSheet.open(QIODevice::ReadOnly);
		if(!styleSheet.isOpen()) {
			continue;
		}

		Theme theme { styleSheet.readAll(), basePath_ + "/themes/" + themeName + "/resources.rcc" };
		themes_.insert(std::pair<QString, Theme>(themeName, theme));
	}
}

std::map<QString, a2img::Theme> a2img::ThemeManager::themes()
{
	return themes_;
}

a2img::Theme a2img::ThemeManager::theme(QString name)
{
	return themes_.at(name);
}

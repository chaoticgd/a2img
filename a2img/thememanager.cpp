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

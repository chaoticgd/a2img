#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <vector>
#include <map>

#include <QString>

namespace a2img {
	struct Theme {
		QString styleSheet;
		QString resourcePath;
	};

	class ThemeManager
	{
	public:
		ThemeManager(QString basePath);

		void reload();

		std::map<QString, Theme> themes();
		Theme theme(QString name);

	private:
		// Directory containing themes folder (e.g. '~/.config/a2img/').
		QString basePath_;

		std::map<QString, Theme> themes_;
	};
}

#endif // THEMEMANAGER_H

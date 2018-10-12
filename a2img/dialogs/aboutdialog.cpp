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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <vector>

#include "application.h"

typedef std::pair<QString, std::function<void(a2img::Application*, std::function<void(QString)>)>> DetailsCategory;

void getDetailsA2img(a2img::Application*, std::function<void(QString)> callback)
{
	callback(
		"Application Version: " + APPLICATION_VERSION.toString() + "\n"
		"Effects API Version: " + EFFECTS_API_VERSION.toString() + "\n"
		"File Format Version: " + FILE_FORMAT_VERSION.toString()
	);
}

void getDetailsOpenGl(a2img::Application* app, std::function<void(QString)> callback)
{
	app->canvas()->draw([=](a2img::GlFunctions* gl) {
		callback(
			"Vendor: " + gl->getString(GL_VENDOR) + "\n"
			"Renderer: " + gl->getString(GL_RENDERER) + "\n"
			"Version: " + gl->getString(GL_VERSION) + "\n"
			"Shading Language Version: " + gl->getString(GL_SHADING_LANGUAGE_VERSION) + "\n"
			"Extensions: " + gl->getString(GL_EXTENSIONS)
		);
	});
}

void getDetailsQt(a2img::Application*, std::function<void(QString)> callback)
{
	callback(
		"Version: " + QString(QT_VERSION_STR)
	);
}

void getDetailsLibArchive(a2img::Application*, std::function<void(QString)> callback)
{
	QString version = QString::number(ARCHIVE_VERSION_NUMBER);
	QString major = QString::number(version.mid(0, 1).toInt());
	QString minor = QString::number(version.mid(1, 3).toInt());
	QString patch = QString::number(version.mid(4, 3).toInt());
	callback(
		"Version: " + major + "." + minor + "." + patch
	);
}

AboutDialog::AboutDialog(a2img::Application* app, QWidget* parent) :
	QDialog(parent),
	ui_(new Ui::AboutDialog)
{
	ui_->setupUi(this);

	MainWindow* window = static_cast<MainWindow*>(parent);

	// Display the logo.
	QImage logo;
	if(window->isDarkThemeInUse()) {
		logo.load(":/icons/logoTitleDark.svg");
	} else {
		logo.load(":/icons/logoTitle.svg");
	}
	ui_->logo->setPixmap(QPixmap::fromImage(logo));

	// Populate details category list.
	const std::vector<DetailsCategory> categories = {
		{ "a2img", getDetailsA2img },
		{ "OpenGL", getDetailsOpenGl },
		{ "Qt", getDetailsQt },
		{ "libarchive", getDetailsLibArchive }
	};

	for(auto category : categories) {
		ui_->detailsCategoryList->addItem(category.first);
	}

	QObject::connect(
		ui_->detailsCategoryList, &QListWidget::itemSelectionChanged,
		[=]() {
			QString selected = ui_->detailsCategoryList->currentItem()->text();
			auto selectedCategory = std::find_if(
				categories.begin(), categories.end(),
				[=](DetailsCategory pair) { return pair.first == selected; }
			);
			selectedCategory->second(app, [=](QString output) {
				ui_->detailsText->setPlainText(output);
			});
		}
	);
}

AboutDialog::~AboutDialog()
{
	delete ui_;
}

void AboutDialog::replaceTemplate(QString original, QString replacement)
{
	ui_->description->setText(ui_->description->text().replace(original, replacement));
}

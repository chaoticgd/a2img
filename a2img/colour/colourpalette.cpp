#include "colourpalette.h"

#include <QPushButton>
#include <QScrollArea>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "application.h"

a2img::ColourPalette::ColourPalette(Application* app)
	: app_(app),
	  numColumns_(6),
	  layout_(new QGridLayout)
{
	setLayout(new QVBoxLayout);

	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setWidgetResizable(true);
	scrollArea->setMinimumHeight(192);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setFrameStyle(0);
	layout()->addWidget(scrollArea);

	paletteBox_ = new QWidget;
	paletteBox_->setLayout(layout_);
	scrollArea->setWidget(paletteBox_);

	layout_->setColumnStretch(6, 1);
	layout_->setRowStretch(6, 1);
	layout_->setVerticalSpacing(8);

	updateGrid();

	QWidget* importExportBox = new QWidget;
	layout()->addWidget(importExportBox);

	QHBoxLayout* importExportLayout = new QHBoxLayout;
	importExportBox->setLayout(importExportLayout);

	QPushButton* importButton = new QPushButton("Import");
	QObject::connect(importButton, &QPushButton::clicked, [=]() {
		importPalette();
	});
	importExportLayout->addWidget(importButton);

	QPushButton* exportButton = new QPushButton("Export");
	QObject::connect(exportButton, &QPushButton::clicked, [=]() {
		exportPalette();
	});
	importExportLayout->addWidget(exportButton);
}

QString a2img::ColourPalette::displayName()
{
	return "Palette";
}

QString a2img::ColourPalette::iconPath()
{
	return ":/icons/colourPalette.svg";
}

void a2img::ColourPalette::setColour(QColor)
{

}

void a2img::ColourPalette::onColourChanged(std::function<void(QColor)> callback)
{
	callback_ = callback;
}

void a2img::ColourPalette::updateGrid()
{
	// Delete old children.
	QList<QPushButton*> oldChildren =
		paletteBox_->findChildren<QPushButton*>();
	for(QPushButton* child : oldChildren) {
		delete child;
	}

	// Create new children.
	unsigned int i;
	for(i = 0; i < colours_.size(); i++) {
		QColor colour = colours_[i];

		QPushButton* colourButton = new QPushButton(" ");
		colourButton->setFlat(true);
		colourButton->setMaximumWidth(32);

		// Change the colour.
		QPalette palette;
		palette.setColor(QPalette::Button, colour);
		colourButton->setAutoFillBackground(true);
		colourButton->setPalette(palette);

		layout_->addWidget(colourButton, i / numColumns_, i % numColumns_);

		QObject::connect(colourButton, &QPushButton::clicked, [=]() {
			callback_(colour);
		});

		colourButton->setContextMenuPolicy(Qt::CustomContextMenu);
		QObject::connect(colourButton, &QPushButton::customContextMenuRequested,
			[=](const QPoint& pos) {
				QMenu* contextMenu = new QMenu;

				// Insert Actions

				auto inserColour = [=](int offset) {
					QColor col = app_->colourManager().getColour();
					colours_.insert(colours_.begin() + i + offset, col);
					updateGrid();
				};

				QAction* insertBeforeAction = contextMenu->addAction("Insert Before");
				QObject::connect(insertBeforeAction, &QAction::triggered, this, [=]() {
					inserColour(0);
				});

				QAction* insertAfterAction = contextMenu->addAction("Insert After");
				QObject::connect(insertAfterAction, &QAction::triggered, this, [=]() {
					inserColour(1);
				});

				// Replace Actions

				QAction* replaceAction = contextMenu->addAction("Replace");
				QObject::connect(replaceAction, &QAction::triggered, this, [=]() {
					colours_[i] = app_->colourManager().getColour();
					updateGrid();
				});

				// Remove Action

				QAction* removeAction = contextMenu->addAction("Remove");
				QObject::connect(removeAction, &QAction::triggered, this, [=]() {
					colours_.erase(colours_.begin() + i);
					updateGrid();
				});


				QPoint globalPos = colourButton->mapToGlobal(pos);
				contextMenu->exec(globalPos);
			}
		);
	}

	QPushButton* addColourButton = new QPushButton("+");
	addColourButton->setMaximumWidth(32);
	layout_->addWidget(addColourButton, i / numColumns_, i % numColumns_);

	QObject::connect(addColourButton, &QPushButton::clicked, [=]() {
		colours_.push_back(app_->colourManager().getColour());
		updateGrid();
	});
}

void a2img::ColourPalette::importPalette()
{
	// Display the dialog.
	QFileDialog dialog(app_->window());

	dialog.setNameFilter("a2img Palette File (*.a2p)");

	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);

	if(!dialog.exec()) {
		return;
	}

	// Import the data.
	const QStringList fileNames = dialog.selectedFiles();
	const QString name = fileNames[0];

	colours_.clear();

	QFile file(name);
	file.open(QIODevice::ReadOnly);

	if(!file.isOpen()) {
		return;
	}

	QTextStream stream(&file);
	while(!stream.atEnd()) {
		QString line = stream.readLine();
		QStringList channels = line.split(',');

		if(channels.length() != 4) {
			break;
		}

		colours_.push_back(QColor {
			channels[0].toInt(),
			channels[1].toInt(),
			channels[2].toInt(),
			channels[3].toInt()
		});
	}

	updateGrid();
}

void a2img::ColourPalette::exportPalette()
{
	// Display the dialog.
	QFileDialog dialog(app_->window());

	dialog.setNameFilter("a2img Palette File (*.a2p)");

	dialog.setViewMode(QFileDialog::Detail);
	dialog.setAcceptMode(QFileDialog::AcceptSave);

	if(!dialog.exec()) {
		return;
	}

	// Generate the data.
	QString data;
	for(QColor colour : colours_) {
		data +=
			QString::number(colour.red()) + ", " +
			QString::number(colour.green()) + ", " +
			QString::number(colour.blue()) + ", " +
			QString::number(colour.alpha()) + "\n";
	}

	// Save the file.
	const QStringList fileNames = dialog.selectedFiles();
	const QString name = fileNames[0];

	QFile file(name);
	file.open(QIODevice::WriteOnly);
	file.write(data.toStdString().c_str(), data.size());
}

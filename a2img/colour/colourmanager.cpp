#include "colourmanager.h"

#include <QOpenGLFramebufferObject>
#include <QDockWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "application.h"
#include "util/rectangle.h"
#include "rgbcolourpicker.h"
#include "hsvcolourpicker.h"
#include "colourpalette.h"

a2img::ColourManager::ColourManager(Application* app)
	: app_(app),
	  ignoreColourChange_(false)
{
	loadColourPanel();
}

QColor a2img::ColourManager::getColour()
{
	return colour_;
}

void a2img::ColourManager::setColour(QColor newColour)
{
	colour_ = newColour;
}

void a2img::ColourManager::loadColourPanel()
{
	colourPickers_.emplace_back(new RGBColourPicker(app_));
	colourPickers_.emplace_back(new HSVColourPicker(app_));
	colourPickers_.emplace_back(new ColourPalette(app_));

	QDockWidget* colourPanel = new QDockWidget("Colour");
	colourPanel->setMinimumWidth(256);
	colourPanel->layout()->setMargin(0);
	app_->window()->addDockWidget(Qt::RightDockWidgetArea, colourPanel);

	QWidget* outerBox = new QWidget;
	outerBox->setLayout(new QVBoxLayout);
	outerBox->layout()->setMargin(0);
	colourPanel->setWidget(outerBox);

	QWidget* navBox = new QWidget;
	navBox->setLayout(new QHBoxLayout);
	outerBox->layout()->addWidget(navBox);

	QScrollArea* bodyScroll = new QScrollArea;
	bodyScroll->setWidgetResizable(true);
	bodyScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	bodyScroll->setFrameStyle(0);
	outerBox->layout()->addWidget(bodyScroll);

	QWidget* bodyBox = new QWidget;
	bodyScroll->setWidget(bodyBox);

	QVBoxLayout* bodyBoxLayout = new QVBoxLayout;
	bodyBoxLayout->setMargin(0);
	bodyBox->setLayout(bodyBoxLayout);

	for(ColourPicker* colourPicker : colourPickers_) {
		colourPicker->hide();
		bodyBox->layout()->addWidget(colourPicker);

		QPushButton* pickerButton = new QPushButton;
		pickerButton->setText(colourPicker->displayName());
		pickerButton->setIcon(QIcon(colourPicker->iconPath()));
		pickerButton->setCheckable(true);
		navBox->layout()->addWidget(pickerButton);

		QObject::connect(pickerButton, &QPushButton::toggled,
						 colourPicker, &QWidget::setVisible);

		colourPicker->onColourChanged([=](QColor newColour) {
			if(ignoreColourChange_) {
				return;
			}

			colour_ = newColour;

			ignoreColourChange_ = true;

			// Update the other colour pickers.
			for(ColourPicker* currentPicker : colourPickers_) {
				if(currentPicker != colourPicker) {
					currentPicker->setColour(newColour);
				}
			}

			ignoreColourChange_ = false;
		});

		// Show the RGB colour picker by default.
		if(colourPicker == colourPickers_[0]) {
			pickerButton->setChecked(true);
		}
	}

	// Add spacer to the end.
	bodyBoxLayout->addStretch(1);
}

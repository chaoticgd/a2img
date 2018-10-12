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

#include "effectmanager.h"

#include <QDir>
#include <QDockWidget>
#include <QToolBox>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>

#include "application.h"
#include "vectorutil.h"
#include "dialogs/errorreportdialog.h"

a2img::EffectManager::EffectManager(Application* app)
	: app_(app),
	  selectedQueuedCommand_(nullptr)
{
	reload();

	loadPanel();
}


std::vector<a2img::Effect*> a2img::EffectManager::effects()
{
	return uniqueVectorToRaw(effects_);
}

a2img::Effect* a2img::EffectManager::effect(QString package, QString name)
{
	for(std::unique_ptr<Effect>& e : effects_) {
		if(	e->package() == package &&
			e->name() == name) {

			return e.get();
		}
	}

	QString errorMessage
		= "Effect " + package + "/" + name + " not loaded.";
	throw InvalidEffectException(errorMessage.toStdString().c_str());
}

void a2img::EffectManager::reload()
{
	// Remove effect previously loaded.
	effects_.clear();

	const auto flags = QDir::Dirs | QDir::NoDotAndDotDot;

	QDir packagesDir(EFFECTS_DIRECTORY);
	QStringList packageNames = packagesDir.entryList(flags);
	for(QString package : packageNames) {
		QDir effectsDir(QString(EFFECTS_DIRECTORY "/") + package);
		QStringList effectNames = effectsDir.entryList(flags);
		for(QString effectName : effectNames) {
			std::unique_ptr<Effect> e;

			try {
				e.reset(new Effect(app_, package, effectName));
				effects_.emplace_back(std::move(e));
			} catch(InvalidEffectException ex) {
				QString message = "Failed to load effect " + package + ":" + effectName
								+ " due to InvalidEffectException: " + ex.what();
				ErrorReportDialog* dialog = new ErrorReportDialog(message);
				dialog->show();
			}
		}
	}

	app_->window()->loadEffectsMenu();

	// Compile and link the effect shaders.
	app_->canvas()->draw([=](GlFunctions*) {
		for(std::unique_ptr<Effect>& e : effects_) {
			try {
				e->compileShaders();
			} catch(InvalidEffectException ex) {
				QString message = "Failed to load effect "+ e->package() + ":" + e->name()
								+ " due to InvalidEffectException: " + ex.what();
				ErrorReportDialog* dialog = new ErrorReportDialog(message);
				dialog->show();
			}
		}
	});

	if(app_->documentManager().isDocumentOpen()) {
		// Add any effect properties that may not
		// have existed when the effect was initially applied,
		// remove those that no longer exist,
		// or otherwise update their state.
		for(Layer* layer : app_->documentManager().currentDocument()->layers()) {
			for(TextureStorageCommand* command : layer->queuedCommands()) {
				EffectCommand* effect = dynamic_cast<EffectCommand*>(command);
				if(effect != nullptr) {
					effect->validateProperties();
				}
			}
		}

		updatePanel();
	}
}

void a2img::EffectManager::updatePanel()
{
	std::vector<TextureStorageCommand*> commands;

	if(	app_->documentManager().isDocumentOpen()) {
		Document* doc = app_->documentManager().currentDocument();
		if(doc->isSelectedLayerValid()) {
			commands = doc->selectedLayer()->queuedCommands();
		}
	}

	QToolBox* toolBox = new QToolBox;
	effectInput_->setWidget(toolBox);

	QObject::connect(toolBox, &QToolBox::currentChanged, [=](int index) {
		Layer* layer = app_->documentManager().currentDocument()->selectedLayer();
		if(layer->queuedCommands().size() <= index) {
			qWarning() << "Attempted to select an out-of-range pending effect.";
			return;
		}
		selectedQueuedCommand_ = layer->queuedCommands()[index];
	});

	// Since tha above QObject::connect call repeatadly sets
	// selectedPendingEffect_, we need to store the actual value here.
	TextureStorageCommand* prevSelectedQueuedCommand = selectedQueuedCommand_;
	size_t newCurrentIndex = 0;

	for(size_t i = 0; i < commands.size(); i++) {
		TextureStorageCommand* command = commands[i];

		QWidget* effectBox = new QWidget;
		command->populateInputPanel(effectBox);
		toolBox->addItem(effectBox, command->text());

		if(command == prevSelectedQueuedCommand) {
			newCurrentIndex = i;
		}
	}

	toolBox->setCurrentIndex(static_cast<int>(newCurrentIndex));
}

void a2img::EffectManager::loadPanel()
{
	QDockWidget* effectPanel = new QDockWidget;
	effectPanel->setWindowTitle("Queued Operations");
	app_->window()->addDockWidget(Qt::RightDockWidgetArea, effectPanel);

	QWidget* outerBox = new QWidget;
	outerBox->setLayout(new QVBoxLayout);
	effectPanel->setWidget(outerBox);

	effectInput_ = new QScrollArea;
	effectInput_->setWidgetResizable(true);
	effectInput_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	effectInput_->setFrameStyle(0);
	outerBox->layout()->addWidget(effectInput_);

	QWidget* buttonBox = new QWidget;
	buttonBox->setLayout(new QHBoxLayout);
	buttonBox->layout()->setMargin(0);
	outerBox->layout()->addWidget(buttonBox);

	// Left Column

	QWidget* orderBox = new QWidget;
	orderBox->setLayout(new QVBoxLayout);
	orderBox->layout()->setMargin(0);
	buttonBox->layout()->addWidget(orderBox);

	auto connectMoveButton = [=](QPushButton* button, bool towardsEnd) {
		QObject::connect(button, &QPushButton::clicked, [=]() {
			if(!app_->documentManager().isDocumentOpen() || selectedQueuedCommand_ == nullptr) {
				return;
			}
			Document* doc = app_->documentManager().currentDocument();
			doc->selectedLayer()->moveQueuedCommand(selectedQueuedCommand_, towardsEnd);
		});
	};

	QPushButton* upButton = new QPushButton("Up");
	orderBox->layout()->addWidget(upButton);
	connectMoveButton(upButton, false);

	QPushButton* downButton = new QPushButton("Down");
	orderBox->layout()->addWidget(downButton);
	connectMoveButton(downButton, true);

	// Right Column

	QWidget* controlBox = new QWidget;
	controlBox->setLayout(new QVBoxLayout);
	controlBox->layout()->setMargin(0);
	buttonBox->layout()->addWidget(controlBox);

	QPushButton* removeButton = new QPushButton("Remove");
	controlBox->layout()->addWidget(removeButton);

	QObject::connect(removeButton, &QPushButton::clicked, [=]() {
		if(!app_->documentManager().isDocumentOpen() || selectedQueuedCommand_ == nullptr) {
			return;
		}
		Document* doc = app_->documentManager().currentDocument();
		doc->selectedLayer()->removeQueuedCommand(selectedQueuedCommand_);

		auto pendingEffects = doc->selectedLayer()->queuedCommands();
		if(pendingEffects.size() >= 1) {
			selectedQueuedCommand_ = pendingEffects[0];
		} else {
			selectedQueuedCommand_ = nullptr;
		}
	});

	QPushButton* applyButton = new QPushButton("Apply All");
	controlBox->layout()->addWidget(applyButton);

	QObject::connect(applyButton, &QPushButton::clicked, [=]() {
		if(!app_->documentManager().isDocumentOpen()) {
			return;
		}
		Document* doc = app_->documentManager().currentDocument();
		doc->selectedLayer()->applyEffects();
	});
}

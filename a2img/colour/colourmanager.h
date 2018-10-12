#ifndef COLOURMANAGER_H
#define COLOURMANAGER_H

#include <vector>

#include <QColor>

#include "colourpicker.h"

namespace a2img {
	class Application;

	// Manages colour picker panel UI and image masks.
	class ColourManager
	{
	public:
		ColourManager(Application* app);

		QColor getColour();
		void setColour(QColor newColour);

	protected:
		void loadColourPanel();

	private:
		Application* app_;

		QColor colour_;

		std::vector<ColourPicker*> colourPickers_;

		// Used to disable onColourChanged callbacks when setColour is being run.
		bool ignoreColourChange_;
	};
}

#endif // COLOURMANAGER_H

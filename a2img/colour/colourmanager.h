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

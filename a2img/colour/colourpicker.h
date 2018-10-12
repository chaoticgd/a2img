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

#ifndef COLOURPICKER_H
#define COLOURPICKER_H

#include <string>
#include <functional>

#include <QColor>
#include <QWidget>

namespace a2img {
	class ColourPicker : public QWidget
	{
	public:
		virtual QString displayName() = 0;
		virtual QString iconPath() = 0;

		virtual void setColour(QColor newColour) = 0;
		virtual void onColourChanged(std::function<void(QColor)> callback) = 0;
	};
}

#endif // COLOURPICKER_H

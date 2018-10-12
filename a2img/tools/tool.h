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

#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <QKeySequence>
#include <QGraphicsItem>

namespace a2img {
	class Application;

	// Pure virtual base class that all tools inherit from.
	class Tool : public QGraphicsItem
	{
	protected:
		Tool(Application* app);
	public:

		QRectF boundingRect() const;
		void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) {}


		// Returns the text to be displayed next to the tools icon.
		virtual QString displayName() = 0;

		// Returns the text resource path of the tools icon.
		virtual QString iconPath() = 0;


		// Called when the tool is selected by the user.
		virtual void enabled() = 0;

		// Called when the tool is deselected by the user.
		virtual void disabled() = 0;

	protected:
		Application* app();

	private:
		Application* app_;
	};
}

#endif // TOOL_H

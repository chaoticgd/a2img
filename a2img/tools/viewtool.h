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

#ifndef VIEWTOOL_H
#define VIEWTOOL_H

#include "tool.h"
#include "../minimap.h"

namespace a2img {
	class Application;

	class ViewTool : public Tool
	{
	public:
		ViewTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();

	private:
		MiniMap* miniMap_;
	};
}

#endif // VIEWTOOL_H

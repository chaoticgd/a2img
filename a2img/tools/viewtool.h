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

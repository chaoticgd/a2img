#ifndef LASSOTOOL_H
#define LASSOTOOL_H

#include "tool.h"

namespace a2img {
	class LassoTool : public Tool
	{
	public:
		LassoTool(Application* app);

		QString displayName();
		QString iconPath();

		void enabled();
		void disabled();
	};
}

#endif // LASSOTOOL_H

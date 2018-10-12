#include "lassotool.h"

a2img::LassoTool::LassoTool(Application* app)
	: Tool(app)
{

}

QString a2img::LassoTool::displayName()
{
	return "Lasso";
}

QString a2img::LassoTool::iconPath()
{
	return ":/icons/lassoTool.svg";
}

void a2img::LassoTool::enabled()
{

}

void a2img::LassoTool::disabled()
{

}

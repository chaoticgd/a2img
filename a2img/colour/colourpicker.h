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

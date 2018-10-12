#ifndef COLOURPALETTE_H
#define COLOURPALETTE_H

#include <functional>
#include <vector>

#include <QColor>
#include <QSlider>
#include <QGridLayout>

#include "colourpicker.h"
#include "../sliderbox.h"

namespace a2img {
	class Application;

	class ColourPalette : public ColourPicker
	{
	public:
		ColourPalette(Application* app);

		QString displayName();
		QString iconPath();

		void setColour(QColor);
		void onColourChanged(std::function<void(QColor)> callback);

		void updateGrid();

	protected:
		void importPalette();
		void exportPalette();

	private:
		Application* app_;

		int numColumns_;
		std::vector<QColor> colours_;
		std::function<void(QColor)> callback_;
		QWidget* paletteBox_;
		QGridLayout* layout_;
	};
}
#endif // COLOURPALETTE_H

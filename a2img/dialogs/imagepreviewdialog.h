#ifndef IMAGEPREVIEWDIALOG_H
#define IMAGEPREVIEWDIALOG_H

#include <QFileDialog>

namespace a2img {
	class ImagePreviewDialog : public QFileDialog
	{
	public:
		ImagePreviewDialog(QWidget* parent);
	};
}

#endif // IMAGEPREVIEWDIALOG_H

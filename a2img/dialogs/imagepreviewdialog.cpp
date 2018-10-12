#include "imagepreviewdialog.h"

#include <QLabel>
#include <QLayout>

a2img::ImagePreviewDialog::ImagePreviewDialog(QWidget* parent)
	: QFileDialog(parent)
{
	QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());

	QLabel* previewImageWidget = new QLabel;
	previewImageWidget->setMinimumSize(100, 100);
	gridLayout->addWidget(previewImageWidget, 0, 3, 2, 1);

	connect(this, &QFileDialog::currentChanged, [=](const QString& path) {
		QImage image(path);
		if(image.isNull()) {
			previewImageWidget->setPixmap(QPixmap());
		} else {
			previewImageWidget->setPixmap(QPixmap::fromImage(image.scaled(100, 100, Qt::KeepAspectRatio)));
		}
	});
}

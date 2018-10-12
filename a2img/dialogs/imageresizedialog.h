#ifndef IMAGERESIZEDIALOG_H
#define RESIZEIMAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeImageDialog;
}

namespace a2img {
    class Application;
}

class ImageResizeDialog : public QDialog
{
    Q_OBJECT

public:
	explicit ImageResizeDialog(a2img::Application* app, QWidget* parent = nullptr);
	~ImageResizeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ResizeImageDialog* ui_;

    a2img::Application* app_;
};

#endif // RESIZEIMAGEDIALOG_H

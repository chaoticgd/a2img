#include "imageresizedialog.h"
#include "ui_imageresizedialog.h"

#include "application.h"
#include "commands/imageresizecommand.h"

ImageResizeDialog::ImageResizeDialog(a2img::Application* app, QWidget* parent) :
    QDialog(parent),
    ui_(new Ui::ResizeImageDialog),
    app_(app)
{
    ui_->setupUi(this);
}

ImageResizeDialog::~ImageResizeDialog()
{
    delete ui_;
}

void ImageResizeDialog::on_buttonBox_accepted()
{
	a2img::Document* doc = app_->documentManager().currentDocument();
    int x = ui_->widthBox->value(), y = ui_->heightBox->value();
	bool stretch = ui_->modeStretch->isChecked();
	bool smooth = ui_->qualityInterpolated->isChecked();

	a2img::ImageResizeCommand* command = new a2img::ImageResizeCommand(app_, doc, x, y, stretch, smooth);
	doc->pushUndoCommand(command);
}

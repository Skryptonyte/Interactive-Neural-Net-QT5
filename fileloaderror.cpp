#include "fileloaderror.h"
#include "ui_fileloaderror.h"

fileLoadError::fileLoadError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fileLoadError)
{
    ui->setupUi(this);
}

fileLoadError::~fileLoadError()
{
    delete ui;
}

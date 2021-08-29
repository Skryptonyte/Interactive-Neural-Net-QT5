#include "uninitializedform.h"
#include "ui_uninitializedform.h"

uninitializedForm::uninitializedForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uninitializedForm)
{
    ui->setupUi(this);
}

uninitializedForm::~uninitializedForm()
{
    delete ui;
}

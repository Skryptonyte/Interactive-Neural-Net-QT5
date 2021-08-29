#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QFile>
#include <QBitmap>
#include <QTextStream>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
}

AboutWindow::~AboutWindow()
{
    ui->label->setScaledContents(true);
    delete ui;
}

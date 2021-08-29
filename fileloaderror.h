#ifndef FILELOADERROR_H
#define FILELOADERROR_H

#include <QDialog>

namespace Ui {
class fileLoadError;
}

class fileLoadError : public QDialog
{
    Q_OBJECT

public:
    explicit fileLoadError(QWidget *parent = nullptr);
    ~fileLoadError();

private:
    Ui::fileLoadError *ui;
};

#endif // FILELOADERROR_H

#ifndef TESTDATAWINDOW_H
#define TESTDATAWINDOW_H

#include <QDialog>

namespace Ui {
class testDataWindow;
}

class testDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit testDataWindow(QWidget *parent = nullptr);
    ~testDataWindow();

public slots:
    void testNNData();
private:
    Ui::testDataWindow *ui;
};

#endif // TESTDATAWINDOW_H

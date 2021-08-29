#ifndef TRAINDATAWINDOW_H
#define TRAINDATAWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <QDebug>
namespace Ui {
class trainDataWindow;
}

class trainDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit trainDataWindow(QWidget *parent = nullptr);
    ~trainDataWindow();

public slots:
    void addBatch();
    void removeBatch();
    void selectBatch(int row);

    void trainNetwork();
    void checked();
    // Overloaded slot
    void reject();
private:
    Ui::trainDataWindow *ui;
};

#endif // TRAINDATAWINDOW_H

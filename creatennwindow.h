#ifndef CREATENNWINDOW_H
#define CREATENNWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
namespace Ui {
class createNNWindow;
}

class createNNWindow : public QDialog
{
    Q_OBJECT

public:
    explicit createNNWindow(QWidget *parent = nullptr);
    ~createNNWindow();

public slots:
      void addLayer();
      void removeLayer();

      void selectItem(int row);
      void setNeuronCount(int count);
      void createNN();
private:
    Ui::createNNWindow *ui;
};

#endif // CREATENNWINDOW_H

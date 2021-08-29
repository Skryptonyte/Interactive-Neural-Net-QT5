#ifndef CREATENEURALNETWORK_H
#define CREATENEURALNETWORK_H

#include <QDialog>

namespace Ui {
class CreateNeuralNetwork;
}

class CreateNeuralNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNeuralNetwork(QWidget *parent = nullptr);
    ~CreateNeuralNetwork();

private:
    Ui::CreateNeuralNetwork *ui;
};

#endif // CREATENEURALNETWORK_H

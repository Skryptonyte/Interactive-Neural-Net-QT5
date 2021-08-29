#include "createnn.h"
#include "ui_createneuralnetwork.h"

CreateNeuralNetwork::CreateNeuralNetwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNeuralNetwork)
{
    ui->setupUi(this);
}

CreateNeuralNetwork::~CreateNeuralNetwork()
{
    delete ui;
}

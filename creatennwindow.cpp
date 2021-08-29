#include "creatennwindow.h"
#include "ui_creatennwindow.h"
#include <QDebug>
#include <gsl/gsl_matrix.h>


#define SIGMOID 0
#define RELU 1
#define TANH 2

extern int* nodeCountArray;
extern int activation;
extern gsl_matrix** weightPtrs;
extern int layers;

extern int created;
extern gsl_vector** a;
extern gsl_vector** z;

int layer = 0;
createNNWindow::createNNWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createNNWindow)
{
    ui->setupUi(this);
    ui->neuronsSpinBox->setEnabled(false);

    created = 0;
    if (nodeCountArray){
        free(nodeCountArray);

    }
    nodeCountArray = (int* ) malloc(1);

    ui->layerList->addItem("Input Layer");
    nodeCountArray[0] = 2;
}

createNNWindow::~createNNWindow()
{
    delete ui;
}

void createNNWindow::addLayer(){
    std::string layerName;
    int layerCount = ui->layerList->count()-1;   // Number of hidden layers
    layerName = "Hidden Layer: " + std::to_string(layerCount+1);
    int* newptr = (int* ) realloc(nodeCountArray, ((layerCount)+2)*sizeof(int));
    if (newptr == nullptr){
        printf("BAD ALLOC\n");
        exit(1);
    }else
       {
        nodeCountArray = newptr;
    }
    nodeCountArray[layerCount+1] = 1;

    ui->layerList->addItem(layerName.c_str());
}

void createNNWindow::removeLayer(){
    int layerCount = ui->layerList->count()-1;

    if (layerCount >= 1){
        QListWidgetItem *it = ui->layerList->takeItem(layerCount);
            if (it){
            delete it;
            }
        nodeCountArray[layerCount] = 0;
        int* newptr = (int* ) realloc(nodeCountArray, (layerCount) * sizeof(int));
        if (newptr == nullptr){
            printf("Attempted allocation of size: %d ints\n", layerCount);
            printf("BAD ALLOC\n");
            exit(1);
        }else
           {
            nodeCountArray = newptr;
        }
    }


}

void createNNWindow::selectItem(int row){
        ui->neuronsSpinBox->setEnabled(true);
        int layerNo = row;
        printf("Layer: %d\n",layerNo);

        ui->neuronsSpinBox->setValue(nodeCountArray[row]);
}

void createNNWindow::setNeuronCount(int neuronCount){
       //QListWidgetItem* item = ui->layerList->selectedItems()[0];
       int row = ui->layerList->currentRow();

       nodeCountArray[row] = neuronCount;
}

void createNNWindow::createNN(){

    printf("Creating neural network\n");
    // Initialize weight tables
    int layerCount = ui->layerList->count()-1;
     weightPtrs = (gsl_matrix** ) calloc(layerCount+1,sizeof(gsl_matrix*));
    a = (gsl_vector** ) calloc(layerCount+1,sizeof(gsl_vector*));
    z = (gsl_vector** ) calloc(layerCount+1,sizeof(gsl_vector*));
    // Set activation function
    if (ui->tanh_rb->isChecked()){
        activation = TANH;
    }
    if (ui->sigmoid_rb->isChecked()){
        activation = SIGMOID;
    }
    if (ui->relu_rb->isChecked()){
        activation = RELU;
    }
    qDebug()<<"Activation function: "<<activation<<'\n';

    // Fill randomized weight matrices
    qDebug()<<"------------------- CREATING NETWORK ------------------------\n";
    qDebug()<<"Input layer: "<<"NODES: "<<nodeCountArray[0]<<'\n';
     for (int l = 1; l <= layerCount;l++){
        qDebug() <<"Layer: "<<l<<", NODES: "<<nodeCountArray[l]<<'\n';
        weightPtrs[l] = gsl_matrix_calloc(nodeCountArray[l],nodeCountArray[l-1]);
        for (int i = 0; i < nodeCountArray[l]; i++){
            for (int j = 0; j <nodeCountArray[l-1];j++){
                double t = (double) rand()/RAND_MAX;
                gsl_matrix_set(weightPtrs[l], i, j,2.0*(t-0.5));
                }
            }
        }
     layers = ui->layerList->count()-1;
     qDebug()<<"Layer Count: "<< layers;
     qDebug()<<"----------------------------------------------------------------";

     this->close();
     created = 1;
}


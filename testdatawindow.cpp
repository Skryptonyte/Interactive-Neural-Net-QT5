#include "testdatawindow.h"
#include "ui_testdatawindow.h"
#include <QDebug>
#include <QListWidget>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "neuralnet_core.h"
//extern gsl_vector* forwardPropagate(gsl_vector* inputVector, gsl_matrix** weightPtrs, gsl_vector** a, gsl_vector** z, int* nodeCountArray, int layers, bool training, int activation);

#define SIGMOID 0
#define RELU 1
#define TANH 2

// Table of functions


extern int* nodeCountArray;
extern int layers;
extern int activation;
extern gsl_matrix** weightPtrs;
extern gsl_vector** a;
extern gsl_vector** z;
testDataWindow::testDataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::testDataWindow)
{
    ui->setupUi(this);
    printf("Testing");
    for (int i =0; i < nodeCountArray[0]; i++){
        ui->inputList->addItem("0.0");
        QListWidgetItem* it = ui->inputList->item(i);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
    }

    for (int i =0; i < nodeCountArray[layers]; i++){
        ui->outputList->addItem("0.0");
    }

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(testNNData()));
}

testDataWindow::~testDataWindow()
{
    delete ui;
}

void testDataWindow::testNNData(){
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    ui->inputList->setEnabled(false);
    gsl_vector* inputVector = gsl_vector_calloc(nodeCountArray[0]);

    // Parse input data
    qDebug("-------------- Parsing TEST Data ----------------");
    qDebug("DATA: [");
    for (int i = 0; i < nodeCountArray[0];i++){
        QListWidgetItem* it = ui->inputList->item(i);
        QString data = it->text();
        const char* data_c = qPrintable(data);
        float f = atof(data_c);
        qDebug()<<f<<',';
        gsl_vector_set(inputVector,i,f);
    }
    qDebug("]\n");
    qDebug("--------------- FORWARD PROPAGATE --------------");
    qDebug()<<"Layer Count: "<<layers  <<'\n';
    gsl_vector* outputVector = gsl_vector_calloc(nodeCountArray[layers]);
    outputVector = forwardPropagate(inputVector,weightPtrs,a,z,nodeCountArray,layers,false, activation);

    ui->outputList->clear();
    for (int i =0; i < nodeCountArray[layers]; i++){
        float f = gsl_vector_get(outputVector,i);
        std::string s = std::to_string(f);
        const char* s1 = s.c_str();
        ui->outputList->addItem(s1);
    }

    gsl_vector_free(inputVector);
    gsl_vector_free(outputVector);

    gsl_vector_free(z[0]);
    for (int i = 1; i <= layers; i++){
        gsl_vector_free(a[i]);
        gsl_vector_free(z[i]);
    }

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->inputList->setEnabled(true);

}



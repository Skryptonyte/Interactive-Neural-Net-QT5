#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "creatennwindow.h"
#include "testdatawindow.h"
#include "traindatawindow.h"
#include "uninitializedform.h"
#include "aboutwindow.h"
#include "mainwindow.h"
#include "fileloaderror.h"
#include <stdio.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
extern int created;
extern int layers;
extern int* nodeCountArray;
extern int activation;
extern gsl_matrix** weightPtrs;
extern gsl_vector** a;
extern gsl_vector** z;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateNeuralNetwork(){
    createNNWindow w;
    w.exec();
}

void MainWindow::testNeuralNetwork(){
    if (created == 0){
     uninitializedForm w;
     w.exec();
    }else{
    testDataWindow w;
    w.exec();
    }
}

void MainWindow::trainNeuralNetwork(){
    if (created == 0){
     uninitializedForm w;
     w.exec();
    }else{
    trainDataWindow w;
    w.exec();
    }
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow w;
    w.exec();
}

void MainWindow::on_actionSave_triggered()
{
    QString s = QFileDialog::getSaveFileName(this,"Save File","","INN Files (*.inn);; All Files (*)");
    if (s == NULL){
        return;
    }

    s += ".inn";
    qDebug()<<"Saving to file: "<<s<<'\n';
    FILE* FH = fopen(qPrintable(s),"wb");

    fputc('_',FH);
    fputc('I',FH);
    fputc('N',FH);
    fputc('N',FH);
    fwrite(&layers,sizeof(int),1,FH);
    fwrite(&activation, sizeof(int),1,FH);
    fwrite(nodeCountArray,sizeof(int),layers+1,FH);
    for (int i = 1; i <= layers; i++){
        gsl_matrix_fwrite(FH,weightPtrs[i]);
    }

    fclose(FH);


}

void MainWindow::on_actionLoad_triggered()
{
    QString s = QFileDialog::getOpenFileName(this,"Open File","","INN Files (*.inn);; All Files (*)");
    qDebug("Cleaning up\n");
    FILE* FH = fopen(qPrintable(s),"rb");
    const char* MAGIC = "_INN";
    for (int i = 0; i < 4; i++){
        if (fgetc(FH) != MAGIC[i]){
            qDebug()<<"Invalid or Corrupted INN File (Bad magic number)";
            fileLoadError f;
            f.exec();
            fclose(FH);
            return;
        }
    }
    if (created == 1){
        for (int i = 1; i <= layers; i++){
            gsl_matrix_free(weightPtrs[i]);
        }
        free(weightPtrs);
        free(nodeCountArray);
        free(a);
        free(z);
        created = 0;
        layers = 0;
    }



    qDebug("Reading variables \n");

    fread(&layers, sizeof(int),1,FH);
    fread(&activation, sizeof(int),1,FH);
    nodeCountArray = (int* ) calloc(layers+1,sizeof(int));

    fread(nodeCountArray, sizeof(int),layers+1,FH);

    weightPtrs = (gsl_matrix**) calloc(layers+1,sizeof(gsl_matrix*));
    a = (gsl_vector**) calloc(layers+1,sizeof(gsl_vector*));
    z = (gsl_vector**) calloc(layers+1,sizeof(gsl_vector*));

    qDebug("Reading weights, Layer Count: %d\n",layers);
    for (int i =1; i <= layers; i++){
        qDebug("Layer No. %d\n",i);
        weightPtrs[i] = gsl_matrix_calloc(nodeCountArray[i],nodeCountArray[i-1]);
        gsl_matrix_fread(FH,weightPtrs[i]);
    }

    created = 1;
}

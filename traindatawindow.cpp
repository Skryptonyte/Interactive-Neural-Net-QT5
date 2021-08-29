#include "traindatawindow.h"
#include "ui_traindatawindow.h"

#include <QDebug>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <mgl2/mgl_cf.h>
#include <mgl2/qmathgl.h>

#include <mgl2/qt.h>


#include <qtconcurrentrun.h>
#include <QThread>
#include <QtCharts/QtCharts>
#include <QtCharts/QLineSeries>

#include "neuralnet_core.h"
extern int* nodeCountArray;
extern gsl_matrix** weightPtrs;
extern gsl_vector** a;
extern gsl_vector** z;

extern int layers;
extern int activation;

int batchCount = 0;
int prevRow = -1;
gsl_vector** batchInput = (gsl_vector** ) malloc(1);
gsl_vector** batchExpected = (gsl_vector** ) malloc(1);

int ongoing = 0;

// Table of functions


trainDataWindow::trainDataWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::trainDataWindow)
{
    ui->setupUi(this);
}

trainDataWindow::~trainDataWindow()
{
    delete ui;
}

void trainDataWindow::addBatch(){
    ui->batchList->addItem("Batch Data");

    int count = ui->batchList->count() - 1;
    qDebug()<<"Previous batch data size: "<<count<<", "<<"New Batch Data size: "<<count+1<<'\n';

    count++;
    batchInput = (gsl_vector** ) realloc(batchInput, sizeof(gsl_vector*) * count);
    batchExpected = (gsl_vector** ) realloc(batchExpected, sizeof(gsl_vector*) * count);

    batchInput[count-1] = gsl_vector_calloc(nodeCountArray[0]);
    batchExpected[count-1] = gsl_vector_calloc(nodeCountArray[layers]);

}

void trainDataWindow::selectBatch(int row){
    qDebug("-------------------");
    qDebug()<<"Batch List Size: "<<ui->batchList->count();
    qDebug()<<"Previous row: "<<prevRow<<", New row: "<<row<<'\n';
    if (prevRow >= 0 && !(prevRow  >= ui->batchList->count())){
        qDebug()<<"Updating previously selected batch\n";
        for (int i = 0; i < nodeCountArray[0];i++){
            gsl_vector_set(batchInput[prevRow],i,atof(qPrintable(ui->inputDataList->item(i)->text())));
        }
        for (int i = 0; i < nodeCountArray[layers];i++){
            gsl_vector_set(batchExpected[prevRow],i,atof(qPrintable(ui->expectedDataList->item(i)->text())));
        }
    }else{
        qDebug()<<"First time row count set\n";
    }
    qDebug("------------------------------");
    qDebug()<<"Row selected: "<<row<<'\n';
    ui->inputDataList->clear();

    if (row >= 0 && row < ui->batchList->count()){
    for (int i =0; i < nodeCountArray[0]; i++){
        std::string s = std::to_string(gsl_vector_get(batchInput[row],i));
        ui->inputDataList->addItem(s.c_str());
        QListWidgetItem* it = ui->inputDataList->item(i);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
    }

    ui->expectedDataList->clear();
    for (int i =0; i < nodeCountArray[layers]; i++){
        std::string s = std::to_string(gsl_vector_get(batchExpected[row],i));
        ui->expectedDataList->addItem(s.c_str());
        QListWidgetItem* it = ui->expectedDataList->item(i);
        it->setFlags(it->flags() | Qt::ItemIsEditable);
    }
    prevRow = row;
    }
}


void trainDataWindow::removeBatch(){
    int count = ui->batchList->count();

    if (count <= 1){
        qDebug()<<"Minimum reached\n";
        return;
    }
    delete ui->batchList->takeItem(count-1);
    gsl_vector_free(batchInput[count-1]);
    gsl_vector_free(batchExpected[count-1]);

    batchInput[count-1] = NULL;
    batchExpected[count-1] = NULL;

    qDebug()<<"Previous batch data size: "<<count<<", "<<"New Batch Data size: "<<count-1<<'\n';

    count--;

    batchInput = (gsl_vector** ) realloc(batchInput, sizeof(gsl_vector*) * (count));
    batchExpected = (gsl_vector** ) realloc(batchExpected, sizeof(gsl_vector*) * (count));

}


void trainDataWindow::trainNetwork(){
    ongoing = 1;
    ui->batchList->setEnabled(false);
    ui->inputDataList->setEnabled(false);
    ui->expectedDataList->setEnabled(false);
    ui->learningRateSpin->setEnabled(false);
    ui->epochSpin->setEnabled(false);

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    int prevRow = ui->batchList->currentRow();
    int batchC = ui->batchList->count();
    if (prevRow >= 0){
    for (int i = 0; i < nodeCountArray[0];i++){
        gsl_vector_set(batchInput[prevRow],i,atof(qPrintable(ui->inputDataList->item(i)->text())));
    }
    for (int i = 0; i < nodeCountArray[layers];i++){
        gsl_vector_set(batchExpected[prevRow],i,atof(qPrintable(ui->expectedDataList->item(i)->text())));
    }
    }

    QLineSeries* series = new QLineSeries();
    //series->append(-1,1);
    //series->append(-2.5,2);

    for (int epoch=1; epoch <= ui->epochSpin->value(); epoch++){
        double MSE_avg = 0.0;
        for (int batchCount = 0; batchCount < ui->batchList->count(); batchCount++){
            gsl_vector* outputVector = forwardPropagate(batchInput[batchCount],weightPtrs,a,z,nodeCountArray,layers,false,activation);
            double MSE = backPropagate(outputVector,batchExpected[batchCount],weightPtrs,a,z,nodeCountArray,ui->learningRateSpin->value(),ui->checkBox->isChecked() | ui->checkBox_2->isChecked(),ui->betaSpin->value(),ui->alphaSpin->value(),layers,activation);
            MSE_avg += MSE;
            gsl_vector_free(outputVector);
        }
        MSE_avg /= batchC;

        double progressValue = (floor((double ) ((double) epoch)/((double) ui->epochSpin->value()) * 100));
        printf("%lf\n",MSE_avg);

        series->append(epoch,MSE_avg);

        ui->progressBar->setValue((int) progressValue);
       QCoreApplication::processEvents();
    }

    QChart* chart = new QChart();

    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();

    chart->setTitle("Cost Function");
    chart->legend()->hide();

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *window = new QDialog();
    QHBoxLayout *layout = new QHBoxLayout(window);
    layout->addWidget(chartView);
    window->resize(1000,1000);
    window->exec();


    delete window;

    ui->batchList->setEnabled(true);
    ui->inputDataList->setEnabled(true);
    ui->expectedDataList->setEnabled(true);
    ui->learningRateSpin->setEnabled(true);
    ui->epochSpin->setEnabled(true);

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->checkBox->setEnabled(true);
    ui->checkBox_2->setEnabled(true);
    ongoing = 0;

}

void trainDataWindow::checked(){
    ui->alphaSpin->setEnabled(ui->checkBox->isChecked());
    ui->betaSpin->setEnabled(ui->checkBox_2->isChecked());
}
// Clean up on exit

void trainDataWindow::reject(){
    qDebug()<<"Cleaning up 3rd window\n";

    if (ongoing == 1){
        qDebug("Ongoing backpropagation, ignoring...");
        return;
    }
    disconnect(this,SLOT(selectBatch(int)));
    for (int i = 0; i < ui->batchList->count(); i++){
        delete ui->batchList->takeItem(i);
        gsl_vector_free(batchInput[i]);
        gsl_vector_free(batchExpected[i]);

        batchInput[i] =NULL;
        batchExpected[i] = NULL;
    }

    batchInput = (gsl_vector**)realloc(batchInput,1);
    batchExpected = (gsl_vector**) realloc(batchExpected,1);

    prevRow = -1;
    qDebug()<<"Closing";
    QDialog::reject();
}



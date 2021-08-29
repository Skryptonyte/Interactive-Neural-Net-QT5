



#include "mainwindow.h"
#include <QApplication>

#include <gsl/gsl_matrix.h>
int created = 0;
int* nodeCountArray;
gsl_matrix** weightPtrs;
gsl_vector** a;
gsl_vector** z;
int activation;
int layers;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

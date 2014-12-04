#include <QtGui/QApplication>
#include "mainwindow.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();



    for(int j=0; j < 1000; j++){
        QVector<double> x(15), y(15);
        for(int i=0; i < 16; i++){
            x.push_back(i);
            if(i%3 == 0) y.push_back(j);
            else if(i%3 == 1) y.push_back(j);
            else y.push_back(j);
        }
        w.setGraphData(&x, &y);
        sleep(1);

    }




    
    return a.exec();
}

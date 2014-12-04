#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    addGraph(ui->customPlot);





//    QVector<double> x2(15), y2(15);

//    for(int i=0; i < 16; i++){
//        x2.push_back(i);
//        if(i%3 == 0) y2.push_back(i*1.4);
//        else if(i%3 == 1) y2.push_back(i/1.2);
//        else y2.push_back(i*1.5);
//    }


//    sleep(5);







}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addGraph(QCustomPlot *customPlot)
{


    customPlot->legend->setVisible(false);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;
    customPlot->addGraph();
    customPlot->graph()->setPen(pen);
    customPlot->graph()->setName("lsStepCenter");
    customPlot->graph()->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    customPlot->yAxis->setRange(0, 1000);
    customPlot->xAxis->setRange(0, 16);
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(true);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));

    //customPlot->replot(QCustomPlot::rpImmediate);
}

Ui::MainWindow* MainWindow::getUI(){
    return ui;
}

void MainWindow::setGraphData(QVector<double> *vec_x, QVector<double> *vec_y){

    ui->customPlot->graph()->setData(*vec_x, *vec_y);
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

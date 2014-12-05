#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    addGraph(ui->customPlot);

    x_data_ = new QVector<double>();
    y_data_ = new QVector<double>();



//    QVector<double> x2(15), y2(15);

//    for(int i=0; i < 16; i++){
//        x2.push_back(i);
//        if(i%3 == 0) y2.push_back(i*1.4);
//        else if(i%3 == 1) y2.push_back(i/1.2);
//        else y2.push_back(i*1.5);
//    }


//    sleep(5);


    dataRateTimer = new QTimer(this);
    graph_update_frequency = 5;
    dataRateTimer->start( 1000 / graph_update_frequency );
    connect(this->dataRateTimer, SIGNAL(timeout()), this, SLOT(refreshGraphData()));




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
    customPlot->yAxis->setRange(0, 1);
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

void MainWindow::refreshGraphData(){
    std::cout << "refresh " << std::endl;
    data_mutex.lock();
    ui->customPlot->graph()->setData(*x_data_, *y_data_);
    data_mutex.unlock();
    ui->customPlot->replot(QCustomPlot::rpImmediate);
}

void MainWindow::setData(QVector<double> *vec_x, QVector<double> *vec_y){
    data_mutex.lock();
    x_data_ = vec_x;
    y_data_ = vec_y;
    data_mutex.unlock();
}



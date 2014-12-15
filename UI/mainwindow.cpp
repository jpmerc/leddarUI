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
    plot_min_range = 0;
    plot_max_range = 1;

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
    customPlot->yAxis->setRange(plot_min_range, plot_max_range);
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
    //std::cout << "refresh " << std::endl;
    data_mutex.lock();
    QVector<double> data_x = *x_data_;
    QVector<double> data_y = *y_data_;
    data_mutex.unlock();

    //std::cout <<  "Size(x) = " << data_x.size() <<  "  Size(y) = " << data_y.size() <<  std::endl;

    ui->customPlot->graph()->setData(data_x, data_y);
    ui->customPlot->replot(QCustomPlot::rpImmediate);
    //ui->customPlot->graph()->rescaleAxes(true);
    updateViewRange(&data_y);
    ui->customPlot->yAxis->setRange(plot_min_range, plot_max_range);
}

void MainWindow::setData(QVector<double> *vec_x, QVector<double> *vec_y){
    data_mutex.lock();
    *x_data_ = *vec_x;
    *y_data_ = *vec_y;
    data_mutex.unlock();
}


void MainWindow::updateViewRange(QVector<double> *range_data){

    double max_value = 0;
    for(int i=0; i < range_data->size(); i++){
        if(range_data->at(i) > max_value) max_value = range_data->at(i);
    }

    double max_view_range = 1.1 * max_value;

    // Increase the length of free space at the top if data is greather than what can be shown in plot
    if(max_value >= plot_max_range){
        plot_max_range = max_view_range;
    }

    // Mostly for when the range data becomes shorter than the plot range
    else{
        double plotTopQuartile = 0.75 * plot_max_range;
        if(max_value < plotTopQuartile){
             plot_max_range = max_view_range;
        }
    }

}



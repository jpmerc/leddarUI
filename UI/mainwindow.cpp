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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addGraph(QCustomPlot *customPlot)
{
    demoName = "Line Style Demo";
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    QPen pen;


    customPlot->addGraph();

    customPlot->graph()->setPen(pen);
    customPlot->graph()->setName("lsStepCenter");
    customPlot->graph()->setLineStyle(QCPGraph::lsStepCenter);
    customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    // generate data:
    QVector<int> bins;
    QVector<double> x(15), y(15);

    for(int i=0; i < 16; i++){
        x.push_back(i);
        if(i%3 == 0) y.push_back(i*4);
        else if(i%3 == 1) y.push_back(i/2);
        else y.push_back(i);
    }

    customPlot->graph()->setData(x, y);
    customPlot->graph()->rescaleAxes(true);
    //  }
    // zoom out a bit:
    customPlot->yAxis->scaleRange(1.1, customPlot->yAxis->range().center());
    customPlot->xAxis->scaleRange(1.1, customPlot->xAxis->range().center());
    // set blank axis lines:
    customPlot->xAxis->setTicks(false);
    customPlot->yAxis->setTicks(true);
    customPlot->xAxis->setTickLabels(false);
    customPlot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    customPlot->axisRect()->setupFullAxesBox();
}

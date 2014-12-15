#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <UI/qcustomplot.h>
#include <QMutex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addGraph(QCustomPlot *customPlot);

    void setData(QVector<double> *vec_x, QVector<double> *vec_y);

    void updateViewRange(QVector<double> *range_data);


    Ui::MainWindow* getUI();


private:
    Ui::MainWindow *ui;
    QVector<double> *x_data_;
    QVector<double> *y_data_;
    QMutex data_mutex;

    QTimer *dataRateTimer;
    double graph_update_frequency;

    double plot_min_range;
    double plot_max_range;


private slots:
    void refreshGraphData();

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <UI/qcustomplot.h>

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
    void setGraphData(QVector<double> *vec_x, QVector<double> *vec_y);

    Ui::MainWindow* getUI();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

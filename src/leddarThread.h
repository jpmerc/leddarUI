#ifndef LEDDARTHREAD_H
#define LEDDARTHREAD_H

#include <QThread>
#include "UI/mainwindow.h"
#include <QtGui/QApplication>

class leddarThread : public QThread
{
    Q_OBJECT
    MainWindow* w;
    QApplication* app;
public:
    explicit leddarThread(MainWindow *in_w, QApplication *in_app, QObject *parent = 0);
    
signals:
    
public slots:

protected:
    void run();
    
};


class leddarTimer : public QTimer {
  Q_OBJECT
public:
  explicit leddarTimer(int timerNumber, QObject *parent = 0);

private slots:
  void RoundRobinSlot();
  void LogFileSlot();
};

#endif // LEDDARTHREAD_H

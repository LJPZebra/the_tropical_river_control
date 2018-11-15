#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QElapsedTimer>
#include <iostream>

using namespace std;
namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = nullptr);
    ~PlotWindow();

public slots:
    void appendData(double temperature);
    void clearData();
    void refreshGraph();
signals:
    void refresh();

private:
    QVector<double> temperatures;
    QVector<double> times;
    QElapsedTimer *time ;
    Ui::PlotWindow *ui;
};

#endif // PLOTWINDOW_H

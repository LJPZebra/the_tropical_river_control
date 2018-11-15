#include "plotwindow.h"
#include "ui_plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    ui->setupUi(this);

    // Hide plot window when hide button is pressed
    connect(ui->hide, &QPushButton::clicked, [this]() {
      this->hide();
    });
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clearData()));
    connect(this, SIGNAL(refresh()), this, SLOT(refreshGraph()));


    time = new QElapsedTimer();
    time->start();

    // Setup graphique
    ui->graph->addGraph();
    ui->graph->xAxis->setLabel("Time");
    ui->graph->yAxis->setLabel("Temperature");

}

void PlotWindow::appendData(double temperature) {
    qint64 t = time->elapsed();
    double tMinute = (t*1E-3)/60.;
    times.append(tMinute);
    temperatures.append(temperature);
    emit(refresh()); 
}

void PlotWindow::clearData() {
    temperatures.clear();
    times.clear();
    emit(refresh()); 
}


void PlotWindow::refreshGraph() {
    ui->graph->xAxis->setRange(times.at(0), times.last());
    cout << times.last() << endl;
    ui->graph->yAxis->setRange(15, 35);
    ui->graph->graph(0)->setData(times, temperatures);
    ui->graph->rescaleAxes();
    ui->graph->replot();
}

PlotWindow::~PlotWindow()
{
    delete ui;
}

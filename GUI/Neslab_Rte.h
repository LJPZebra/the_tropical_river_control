#ifndef NESLAB_RTE_H                                                                         
#define NESLAB_RTE_H

#include <QSerialPort>
#include <QDebug>
#include <QThread>

class Neslab_Rte : public QObject {

  Q_OBJECT

private:
  int messageCode;

public:

  Neslab_Rte(QString);
  ~Neslab_Rte();
  QSerialPort *serial;
public slots:
  void read();
  void start();
  void stop();
  void setTemperature(double temperature);
  void getTemperature();
  void getTemperatureSetPoint();
  void sendCommand(QByteArray cmd);

signals:
  void newMessage(QString);  
  void temperature(double);
  void temperatureSet(double);
};
#endif

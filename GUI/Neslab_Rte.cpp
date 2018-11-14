#include "Neslab_Rte.h"
#include <iostream>

using namespace std;

Neslab_Rte::Neslab_Rte(QString portName) {

  serial = new QSerialPort(this);
  serial->setPortName(portName);
  serial->setBaudRate(9600);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);

  if (serial->open(QIODevice::ReadWrite)) {
    qInfo() << "Neslab Rte heater connected"<< serial->portName();
  }
  else{
    qInfo() << "Failed to open port" << serial->portName();
  }
  connect(serial, SIGNAL(readyRead()), this, SLOT(read()));
}

Neslab_Rte::~Neslab_Rte() {
  stop();
}

void Neslab_Rte::read() {

  QByteArray readData = serial->readAll();
   //Wait that the message is complete
  while (serial->waitForReadyRead(10)) { 
      readData.append(serial->readAll());
  }
  QString message(readData);
  double temp = message.split(QRegExp("[\r]"),QString::SkipEmptyParts).at(0).toDouble();           // Delete endline caracters
  emit(newMessage(message));
  
  // Update internal variables
  switch(messageCode) {
    case 0 : 
             emit(temperature(temp));
             break;
    case 1 : emit(temperatureSet(temp));
             break;
}
   
}

void Neslab_Rte::start() {
    serial->write(QByteArray("SO1\r"));
    serial->flush(); 
    QThread::msleep(5);
}


void Neslab_Rte::stop() {
    serial->write(QByteArray("SO0\r"));
    serial->flush(); 
    QThread::msleep(5);
}

void Neslab_Rte::setTemperature(double temperature) {
    cout << temperature << endl;
    QString cmd = "SS" + QString::number(temperature) + "\r";
    QByteArray command = cmd.toUtf8();
    serial->write(command);
    serial->flush(); 
}

void Neslab_Rte::getTemperature() {
    serial->write(QByteArray("RT\r"));
    serial->flush();
    messageCode = 0; 
    QThread::msleep(5);
}

void Neslab_Rte::getTemperatureSetPoint() {
    serial->write(QByteArray("RS\r"));
    serial->flush(); 
    messageCode = 1; 
    QThread::msleep(5);
}

void Neslab_Rte::sendCommand(QByteArray cmd) {
    serial->write(cmd);
    serial->flush(); 
}

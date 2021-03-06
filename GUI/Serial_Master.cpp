#include "Serial_Master.h"

Serial_Master::Serial_Master() {
  checkSerialConnection();
}


void Serial_Master::checkSerialConnection() {
  
  qInfo() <<  "Serial connections";
  const QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
  qInfo() << infos.length() << "serial connections detected";

  // Scan all devices connected and ask id serial if it is an Arduino and if it is not already open.
  for (int i = 0; i < infos.length(); i++) {
    // Note to dev: first version was with =="Arduino" but dev computer get "ARDUINO" 
    // Reimplemented with all lowercase, to be tested on another dev env.
    if ((infos[i].description().left(7).toLower() == "arduino") && !(infos[i].isBusy()) ) {

      qInfo() << "Opening" << infos[i].portName();
      QSerialPort *serial = new QSerialPort(this);
      serial->setPortName(infos[i].portName());
      serial->setBaudRate(115200);
      serial->setDataBits(QSerialPort::Data8);
      serial->setParity(QSerialPort::NoParity);
      serial->setStopBits(QSerialPort::OneStop);
      serial->setFlowControl(QSerialPort::NoFlowControl);
      connect(serial, SIGNAL(readyRead()), this, SLOT(getSerialId()));
      
      // Find the dd of the serial (see Arduino documentation to define an id)
      if (serial->open(QIODevice::ReadWrite)) {
        // This loop is necessary to connect with the Arduino nano.
        // The Arduino Nano need some commands before answering.
        for(int i =0; i < 10; i++){
          serial->write(QByteArray("getId"));
          serial->flush(); 
          QThread::msleep(200);
        }
      }
      
      else{
        qInfo() << "Failed to open port" << serial->portName();
      }
    }
  }
}
  

void Serial_Master::getSerialId() {


  QSerialPort* serial = qobject_cast<QSerialPort*>(sender());               // Find the object that emit the signal that trigger this method.

  QByteArray readData = serial->readAll();
  // Wait for the integrality of the message
  while (serial->waitForReadyRead(0)) { 
      readData.append(serial->readAll());
  }

  QString id(readData);
  id = id.split(QRegExp("[\r\n]"),QString::SkipEmptyParts).at(0);           // Delete endline caracters

  qInfo() << " → " << serial->portName() << "is" << id;

  Serial_Device *device = new Serial_Device(serial, id);

  serialList.insert(id, device);

  disconnect(serial, SIGNAL(readyRead()), this, SLOT(getSerialId()));
  connect(serial, SIGNAL(readyRead()), device, SLOT(readSerialMessage()));
  connect(device, SIGNAL(newMessage(QString, QString)), this, SIGNAL(newMessage(QString, QString)));
}

void Serial_Master::sendSerialCommand(QString serialId, QString commande) {
    if(serialList.contains(serialId)){
      serialList.value(serialId)->sendSerialCommand(commande.toStdString().c_str());
    }
}


QVector<QString> Serial_Master::listSerials() {

  QVector<QString> serialListId;
  for(auto i: serialList.keys()) {
    serialListId.append(QString(i));
  }
  return serialListId;
}



/****************************************************************
Serial_Device class
***************************************************************/


void Serial_Device::readSerialMessage() {


  QByteArray readData = serial->readAll();
  while (serial->waitForReadyRead(0)) { 
      readData.append(serial->readAll());
  }
  QString message(readData);
  // Replace message.split with message.remove for deleting endline characters because sometimes
  // arduino send empty message with endline characteres that triggers a segmentation fault
  // error.
  message.remove(QRegExp("[\r\n]+"));

  emit(newMessage(id, message)); 
}


void Serial_Device::sendSerialCommand(QString commande) {
  serial->write(commande.toStdString().c_str());
  serial->flush();
}


Serial_Device::Serial_Device(QSerialPort* ser, QString serialId) {
  serial = ser;
  id = serialId;
}

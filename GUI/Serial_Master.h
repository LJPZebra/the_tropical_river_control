#ifndef SERIAL_MASTER_H
#define SERIAL_MASTER_H

#include <QDebug>
#include <QMap>
#include <QThread>
#include <QRegExp>
#include <QVector>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <iostream>

using namespace std;

/**
  *  structure to represent the image and all its informations
*/
struct SerialStruct {
    /*@{*/
     QString serialId;        /**< serial Id (see documentation on Arduino protocol standard) */
     QSerialPort *serial;     /**< pointer to the QSerialPort object */
     QString lastMessage;     /**< last message received from the serial */
    /*@}*/
};


/**
 * \class Serial_Master
 *
 * \detailed This class implementes a mean to communicate with several Arduino by serial port.
             The Arduino has to follow a certain standard (see Arduino standard documentation).
             When an object Serial_Master is created, all the serial connections are scanned
             and Arduino device are attributed an id.Then the serial can be used providing
             the id. A list of all the devices are available by the listSerials() method.
             All commands are send by the sendSerialCommand(serialId, command) method.
             All messages from all device are received by the readSerialMessage() method and a 
             signal newMessage(serialId, message) is emitted when a new message is received.
             A copy of the last message can be accessed in the Serial_Master structure.
 *
 * \author Benjamin Gallois
 *
 * \date Date: 2018/10/24
 *
 */
class Serial_Master : public QObject {

  Q_OBJECT

public:
  /**
    *\brief Constructs the Serial_Master object by scanning the serial port connections.
            If the device is an Arduino not busy, it request its id and map id->QSerialObject
            and port->id.
  */
  Serial_Master();

  /**
    *\brief Gets all id of all connected serials. 
    *\returns - QVector(QString) - A list of the id of connected Arduino devices.
  */
  QVector<QString> listSerials();
  
  
  QMap<QString, SerialStruct> serialList;       // QMap to access the seria structure by the serial id


private:
  QMap<QString, QString> portNameToId;          // QMap to access the serial Id by the port name

public slots:
  /**
    *\brief Provides a method to re-check the serial connections after the class initialization. 
  */
  void checkSerialConnection();

  /**
    *\brief Waits for an answer for all connected devices, and, when a new message is received
            emit a signal with the message and the id of the sender.
  */
  void readSerialMessage();

  /**
    *\brief Sends a command to the id serial.
    *\param - QString - serialId is the id of the device.
    *\param - QString - command is the command to send.
  */
  void sendSerialCommand(QString serialId, QString command);

  /**
    *\brief After connection to the serial asks for id and assign
            * id => serial struct with all the information of the serial and the pointer to the             serial connection.
            * port name ==> id necessary to sort received message.
    *\param - QString - serialId is the id of the device.
    *\param - QString - command is the command to send.
  */
  void getSerialId();

  

signals:
  /**
    *\brief This signal is triggered when a new message is received.
    *\param - QString - serialId is the id of the device that has sent the message.
    *\param - QString - message is the message received.
  */
  void newMessage(QString serialId, QString message);

};

#endif


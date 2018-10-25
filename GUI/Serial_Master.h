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



/****************************************************************************
                       Serial_master Class                             

This class implementes a mean to communicate with several serials following
 a certain standard. The serial has to be an Arduino and possesses at
least a function getId. If it possesses in addition a function listCommands
it can be automaticcaly used with the protocol system see documentation.

When an object Serial_Master is created, all the serial connections are scanned
and all Arduino serials are requested an id. Then the serial can be used providing
the id, a list of all the serial id are available with the listSerials() method.

All commands are send by the sendSerialCommand(serialId, command).
All message are received by the function readSerialMessage() and a 
signal newMessage(serialId, message) is emitted when new message is received.
A copy of the last message can be accessed in the Serial_Master structure.
In case of several serials connected, the message has to be sorted manually
outside the class by id.

****************************************************************************/
/**
 * \class Serial_Master
 *
 * \Dev note:
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
    *\brief Constructor that check all serial connections and request an id if it is
            an Arduino device. 
  */
  Serial_Master();

  /**
    *\brief Get all id of all connected serials. 
    *\return QVector(QString) a list of all id.
  */
  QVector<QString> listSerials();
  
  
  QMap<QString, SerialStruct> serialList;       // QMap to access the seria structure by the serial id


private:
  QMap<QString, QString> portNameToId;          // QMap to access the serial Id by the port name

public slots:

  /**
    *\brief Provide a method to recheck the serial connection after the class initialization. 
  */
  void checkSerialConnection();

  /**
    *\brief Wait for an answer for all connected serials and when a new message is received
            emit a signal with the message and the id of the sender serial.
  */
  void readSerialMessage();

  /**
    *\brief Send a command to the id serial.
  */
  void sendSerialCommand(QString serialId, QString command);

  /**
    *\brief After connection to the serial asks for id and assign
            * id => serial struct with all the information of the serial and the pointer to the             serial connection.
            * port name ==> id necessary to sort received message.
    *\param[in] serialId - id of the serial to send the message.
    *\param[in] command - command to send to the serial.
  */
  void getSerialId();

  

signals:
  /**
    *\brief Signael emitted when a new message from serialId is received.
    *\param[in] serialId - id of the serial to send the message.
    *\param[in] message - message received by the serial.
  */
  void newMessage(QString serialId, QString message);

};

#endif


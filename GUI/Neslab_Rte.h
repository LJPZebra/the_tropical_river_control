#ifndef NESLAB_RTE_H                                                                         
#define NESLAB_RTE_H

#include <QSerialPort>
#include <QDebug>
#include <QThread>

/**
 * \class Neslab_Rte
 *
 * \brief The class Neslab_Rte implementes an object to control a Neslab bath heater cooler
 *        equiped with a microprocessor control. See documentation at: https://www.chillercity.com/OPMANUAL/rtexx1m.pdf and https://www.idealvac.com/files/brochures/NesLab_RTE-111_Service_Manual.pdf 
 *
 *
 * \note Getter not implemented
 *
 * \author Benjamin Gallois
 *
 * \date Date: 2018/11/01 
 *
 */
class Neslab_Rte : public QObject {

  Q_OBJECT

private:
  int messageCode;                            // Usefull to sort serial message received

public:

  /**
    *\brief Constructs the Neslab_Rte object with the port serie associated. 
    *\arg QString port serie
  */
  Neslab_Rte(QString);
  
  /**
    *\brief Destructs the Neslab_Rte object, stop the heater. 
  */
  ~Neslab_Rte();

  QSerialPort *serial;


public slots:
  /**
    *\brief Reads message from the heater.
  */
  void read();
  

  /**
    *\brief Sends a message to start the heater.
    *\note Sometimes the heater is long to fully start ~15 secondes.
  */
  
  
  void start();
  /**
    *\brief Sends a message to start the heater.
    *\note Sometimes the heater is long to fully start ~15 secondes.
  */


  /**
    *\brief Sends a message to stop the heater.
  */
  void stop();
  
  
  /**
    *\brief Sends a message to change the set point temperature of the heater.
    *\arg - int - temperature set point.
  */
  void setTemperature(double temperature);
  
  
  /**
    *\brief Sends a message to get the bath temperature of the heater.
  */
  void getTemperature();


  /**
    *\brief Sends a message to get the set point temperature of the heater.
  */
  void getTemperatureSetPoint();
  
  
  /**
    *\brief Sends a message to the heater.
    *\arg - QByteArray - message to send to the heater.
  */
  void sendCommand(QByteArray cmd);

signals:
  /**
    *\brief This signal is triggered when a new message is received from the Neslab_Rte heater
    *\arg - QString - Message 
  */
  void newMessage(QString);  
  
  
  /**
    *\brief This signal is triggered when a new temperature measure of the heater bath is available.
    *\arg - double - Temperature
  */
  void temperature(double);
  
  
  /**
    *\brief This signal is triggered when a new temperature set point of the heater bath is available.
    *\arg - double - Temperature
  */
  void temperatureSet(double);
};
#endif

#ifndef PID_H                                                                         
#define PID_H

#include <QObject>

/**
 * \class Pid
 *
 * \brief The class Pid implementes a PID object that can be used to make a 
 *        control loop feedback mechanism see: https://en.wikipedia.org/wiki/PID_controller.
 *
 *\detailed The class is initialized with the range value for the controller, referes to your controller manual to set them. A target is set with the setTarget method. The P, D and I parameters are set with corresponding setters. A manual tuning is necessary. When a the current value of the parameter measured by the sensor is available, call the computeSetPoint(current value) method to have a new set point value emitted by the newSetPoint(setPoint) signal.  
 *
 * \author Benjamin Gallois
 *
 * \date Date: 2018/11/14 
 *
 */
class Pid : public QObject {
  Q_OBJECT

private:
  double m_error;
  double m_integral;
  double m_maxLimit;
  double m_minLimit;


public:
  double m_target;
  double m_p;
  double m_i;
  double m_d;


  /**
    *\brief Constructs the Pid object by setting the min max range of the controller. 
    *\arg - double - maximal value allowed by the controller.
    *\arg - double - minimal value allowed by the controller.
  */
  Pid(double min, double max);

public slots:
  /**
    *\brief Sets the target value.
    *\arg - double - Target value.
  */
  void setTarget(double);


  /**
    *\brief Sets P parameter.
    *\arg - double - P parameter.
  */
  void setP(double);
  
  
  /**
    *\brief Sets I parameter.
    *\arg - double - I parameter.
  */
  void setI(double);
  
  
  /**
    *\brief Sets D parameter.
    *\arg - double - D parameter.
  */
  void setD(double);
  
  
  /**
    *\brief Computes new set point from current value..
    *\arg - double - current value.
  */
  void computeSetPoint(double current);

signals:
  /**
    *\brief This signal is triggered when a new set point is calculated after PID.
    *\arg - double - New temperature set point.
  */
  void newSetPoint(double);
};
#endif

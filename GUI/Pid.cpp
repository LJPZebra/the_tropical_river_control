#include "Pid.h"
#include <iostream>

Pid::Pid(double min, double max) {

  m_maxLimit = max;
  m_minLimit = min;
  m_p = 0;
  m_i = 0;
  m_d = 0;

}

void Pid::computeSetPoint(double current) {


  double error = m_target - current;
  
  double derivative = (error - m_error)/(0.1/60.);
  m_error = error;

  m_integral +=  error*(0.1/60.);
  
  double setPoint = m_p*error + m_d*derivative + m_i*m_integral;

  // Set the set point parameter in define range
  if (setPoint > m_maxLimit) {
    setPoint = m_maxLimit;
  }
  else if (setPoint < m_minLimit) {
    setPoint = m_minLimit;
  }

  emit(newSetPoint(setPoint));
  std::cout << "Target   " << m_target << std::endl;
  std::cout << "Current   " << current << std::endl;
  m_setPoint = setPoint;
  m_past = current;
}


void Pid::setTarget(double target) {
  m_target = target;
}

void Pid::setP(double p) {
  m_p = p;
}


void Pid::setI(double i) {
  m_i = i;
}

void Pid::setD(double d) {
  m_d = d;
}


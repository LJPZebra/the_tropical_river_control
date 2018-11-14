#include "Pid.h"
#include <iostream>

Pid::Pid(double min, double max) {

  m_maxLimit = max;
  m_minLimit = min;

}

void Pid::computeSetPoint(double current) {

  double error = m_target - current;
  
  double derivative = error - m_error;
  m_error = error;

  m_integral +=  error;
  
  double setPoint = m_p*error + m_d*derivative + m_i*derivative;

  // Set the set point parameter in define range
  if (setPoint > m_maxLimit) {
    setPoint = m_maxLimit;
  }
  else if (setPoint < m_minLimit) {
    setPoint = m_minLimit;
  }

  emit(newSetPoint(setPoint));
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


#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/Sum.hpp>
#include <eeros/control/D.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>

using namespace eeros::control;

class ControlSystem {
 public:
  ControlSystem(double ts);
  
  Constant<> setpoint;
  PeripheralInput<double> enc;
  D<> diff1;
  Sum<2> sum1;
  Gain<> posController;
  D<> diff2;
  Sum<3> sum2;
  Gain<> speedController;
  Gain<> inertia;
  Gain<> invMotConst;
  PeripheralOutput<double> dac;

  TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP_

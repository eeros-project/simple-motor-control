#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/Sum.hpp>
#include <eeros/control/D.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/Switch.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>

using namespace eeros::control;

class ControlSystem {
 public:
  ControlSystem(double ts);
  
  Constant<> setpoint, initSpeed;
  PeripheralInput<double> enc;
  D<> diff1, diff2;
  Switch<> sw;
  Sum<2> sum1;
  Sum<3> sum2;
  Gain<> posController, speedController, inertia, invMotConst;
  PeripheralOutput<double> dac;

  TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP_

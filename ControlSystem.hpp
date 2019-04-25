#ifndef CONTROLSYSTEM_HPP_
#define CONTROLSYSTEM_HPP_

#include <eeros/control/Sum.hpp>
#include <eeros/control/D.hpp>
#include <eeros/control/Gain.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/TimeDomain.hpp>

class ControlSystem {

public:
	ControlSystem(double ts);
	~ControlSystem();
	
	eeros::control::Constant<> setpoint;
	eeros::control::PeripheralInput<double> enc;
	eeros::control::D<> diff1;
	eeros::control::Sum<2> sum1;
	eeros::control::Gain<> posController;
	eeros::control::D<> diff2;
	eeros::control::Sum<3> sum2;
	eeros::control::Gain<> speedController;
	eeros::control::Gain<> inertia;
	eeros::control::Gain<> invMotConst;
	eeros::control::PeripheralOutput<double> dac;

	eeros::control::TimeDomain timedomain;
};

#endif // CONTROLSYSTEM_HPP_

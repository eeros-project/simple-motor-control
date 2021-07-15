#include "ControlSystem.hpp"
#include <eeros/core/Executor.hpp>
#include <math.h>

const double dampingFactor = 0.9;

ControlSystem::ControlSystem(double ts) 
    : setpoint(0.0),
      initSpeed(5),
      enc("q", false),
      sw(0),
      posController(2 * M_PI / (ts * 20) / (2 * dampingFactor)),	// 174.5
      speedController(2 * M_PI / (ts * 20) * 2 * dampingFactor),	//565.48
      inertia(9.49e-7),
      invMotConst(1/16.3e-3),
      dac("motor"),
      timedomain("Main time domain", ts, true) {
  
  setpoint.getOut().getSignal().setName("phi desired");
  setpoint.getOut().getSignal().setName("init speed");
  enc.getOut().getSignal().setName("phi actual");
  sum1.negateInput(1);
  sum1.getOut().getSignal().setName("phi error");	
  posController.getOut().getSignal().setName("pos ctrl out");
  sum2.negateInput(1);
  speedController.getOut().getSignal().setName("speed ctrl out");
  inertia.getOut().getSignal().setName("M");
  invMotConst.getOut().getSignal().setName("i");
  
  diff1.getIn().connect(enc.getOut());
  sum1.getIn(0).connect(setpoint.getOut());
  sum1.getIn(1).connect(enc.getOut());
  posController.getIn().connect(sum1.getOut());
  diff2.getIn().connect(setpoint.getOut());
  sum2.getIn(0).connect(posController.getOut());
  sum2.getIn(1).connect(diff1.getOut());
  sum2.getIn(2).connect(diff2.getOut());
  sw.getIn(0).connect(initSpeed.getOut());
  sw.getIn(1).connect(sum2.getOut());
  speedController.getIn().connect(sw.getOut());
  inertia.getIn().connect(speedController.getOut());
  invMotConst.getIn().connect(inertia.getOut());
  dac.getIn().connect(invMotConst.getOut());
  
  timedomain.addBlock(setpoint);
  timedomain.addBlock(enc);
  timedomain.addBlock(diff1);
  timedomain.addBlock(sum1);
  timedomain.addBlock(diff2);
  timedomain.addBlock(posController);
  timedomain.addBlock(sum2);
  timedomain.addBlock(initSpeed);
  timedomain.addBlock(sw);
  timedomain.addBlock(speedController);
  timedomain.addBlock(inertia);
  timedomain.addBlock(invMotConst);
  timedomain.addBlock(dac); 

  eeros::Executor::instance().add(timedomain);
}


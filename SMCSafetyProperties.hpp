#ifndef SMCSAFETYPROPERTIES_HPP_
#define SMCSAFETYPROPERTIES_HPP_

#include <eeros/safety/SafetyProperties.hpp>
#include <eeros/hal/HAL.hpp>
#include "ControlSystem.hpp"

using namespace eeros::safety;

class SMCSafetyProperties : public SafetyProperties {
  
 public:
  SMCSafetyProperties(ControlSystem& controlSys, double ts);
  
  // Define all possible events
  SafetyEvent doSystemOn;
  SafetyEvent doSystemOff;
  SafetyEvent startControl;
  SafetyEvent startControlDone;
  SafetyEvent stopControlDone;
  SafetyEvent startMoving;
  SafetyEvent stopMoving;
  SafetyEvent doEmergency;
  SafetyEvent resetEmergency;	
  SafetyEvent abort;
  
  // Name all levels
  SafetyLevel slOff;
  SafetyLevel slEmergency;
  SafetyLevel slSystemOn;
  SafetyLevel slStartingControl;
  SafetyLevel slStoppingControl;
  SafetyLevel slPowerOn;
  SafetyLevel slMoving;
  
private:
  // critical outputs
  eeros::hal::Output<bool>* enable;
  
  // critical inputs
  eeros::hal::Input<bool>* emergency;
  eeros::hal::Input<bool>* ready;
    
  ControlSystem& cs;
};

#endif // SMCSAFETYPROPERTIES_HPP_

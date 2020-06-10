#include <iostream>
#include <ostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <eeros/hal/HAL.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/sequencer/Sequencer.hpp>
#include "SMCSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include "MainSequence.hpp"

using namespace eeros;
using namespace eeros::hal;
using namespace eeros::control;
using namespace eeros::safety;
using namespace eeros::logger;
using namespace eeros::sequencer;

const double dt = 0.001;

void signalHandler(int signum){
  SafetySystem::exitHandler();
  Sequencer::instance().abort();
}

int main(int argc, char **argv) {
  StreamLogWriter w(std::cout);
  Logger::setDefaultWriter(&w);
  Logger log;
  w.show();
  
  log.info() << "Simple Motor Controller Demo started...";
  
  log.info() << "Initializing Hardware...";
  HAL& hal = HAL::instance();
  hal.readConfigFromFile(&argc, argv);
  
  // Create the control system
  ControlSystem controlSys(dt);
  
  // Create and initialize a safety system
  SMCSafetyProperties properties(controlSys, dt);
  SafetySystem safetySys(properties, dt);
  controlSys.timedomain.registerSafetyEvent(safetySys, properties.doEmergency);
  signal(SIGINT, signalHandler);

  auto& sequencer = Sequencer::instance();
  MainSequence mainSequence("Main Sequence", sequencer, safetySys, properties, controlSys, 3.14/10);
  sequencer.addSequence(mainSequence);
  mainSequence.start();
  
  auto &executor = Executor::instance();
  executor.setMainTask(safetySys);
  safetySys.triggerEvent(properties.doSystemOn);
  
  executor.run();
  
  mainSequence.waitAndTerminate();
  log.info() << "Example finished...";
  return 0;
}

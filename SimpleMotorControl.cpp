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
  
  Logger::setDefaultStreamLogger(std::cout);
  Logger log = Logger::getLogger();
  
  log.info() << "Simple Motor Controller Demo started...";
  
  log.info() << "Initializing Hardware...";
  HAL& hal = HAL::instance();
  hal.readConfigFromFile(&argc, argv);
  
  // Create the control system
  ControlSystem cs(dt);
  
  // Create and initialize a safety system
  SMCSafetyProperties sp(cs, dt);
  SafetySystem ss(sp, dt);
  cs.timedomain.registerSafetyEvent(ss, sp.doEmergency);
  signal(SIGINT, signalHandler);
  
  auto& sequencer = Sequencer::instance();
  MainSequence mainSequence("Main Sequence", sequencer, ss, sp, cs, 3.14/10);
  mainSequence();
  
  auto &executor = Executor::instance();
  executor.setMainTask(ss);
  ss.triggerEvent(sp.doSystemOn);
  
  executor.run();
  
  sequencer.wait();
  log.info() << "Simple Motor Controller Demo finished...";

  return 0;
}

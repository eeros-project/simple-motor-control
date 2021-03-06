#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "SMCSafetyProperties.hpp"
#include "ControlSystem.hpp"

using namespace eeros::sequencer;
using namespace eeros::safety;
using namespace eeros::logger;

class Move : public Step {
 public:
  Move(std::string name, Sequence* caller, ControlSystem& cs) : Step(name, caller), cs(cs) { }
  int operator() (double pos) {this->pos = pos; return start();}
  int action() {
    cs.setpoint.setValue(pos);
    return 0;
  }
  double pos;
  ControlSystem& cs;
};

class MainSequence : public Sequence {
 public:
  MainSequence(std::string name, Sequencer& seq, SafetySystem& safetySys, SMCSafetyProperties& safetyProp, ControlSystem& cs, double angle) 
     : Sequence(name, seq), ss(safetySys), sp(safetyProp), angle(angle), cs(cs), move("move", this, cs), pause("pause", this) {
    log.info() << "Sequence created: " << name;
  }
  int action() {
    angle = 0;
    while(Sequencer::running) {
      if (ss.getCurrentLevel() == sp.slMoving) {
        angle += 6.28 / 10;
        move(angle);
        pause(1.0);
        log.info() << "pos =  " << cs.enc.getOut().getSignal().getValue();
      }
    }
    return 0;
  }
 private:
  Move move;
  Wait pause;
  double angle;
  SafetySystem& ss;
  ControlSystem& cs;
  SMCSafetyProperties& sp;
};

#endif // MAINSEQUENCE_HPP_ 

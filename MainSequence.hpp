#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
#include <eeros/sequencer/Wait.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include "SMCSafetyProperties.hpp"
#include "ControlSystem.hpp"
#include <unistd.h>
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

class HomingSequence : public Sequence {
 public:
  HomingSequence(std::string name, Sequence* caller, ControlSystem& cs) 
     : Sequence(name, caller, true), cs(cs), pause("pause", this) {
  }
  int action() {
    cs.sw.switchToInput(0);
    pause(5);
    cs.sw.switchToInput(1);
    cs.enc.callInputFeature("resetFqd");
    return 0;
  }
 private:
  ControlSystem& cs;
  Wait pause;
};

class MainSequence : public Sequence {
 public:
  MainSequence(std::string name, Sequencer& seq, SafetySystem& safetySys, SMCSafetyProperties& safetyProp, ControlSystem& cs) 
     : Sequence(name, seq), 
       ss(safetySys), sp(safetyProp), cs(cs), 
       homingSequence("homing", this, cs), 
       move("move", this, cs), 
       pause("pause", this) { }
  int action() {
    angle = 0;   
    diff = 6.28 / 10;
    while(state == SequenceState::running) {
      if(ss.getCurrentLevel() == sp.slHoming) {
        homingSequence();
        sp.homed = true;
        ss.triggerEvent(sp.homingDone);
      }
      if (ss.getCurrentLevel() == sp.slMoving) {
        angle += diff;
        move(angle);
        pause(1.0);
        if (angle > 6.28 || angle < 0) diff = -diff;
        log.info() << "pos =  " << cs.enc.getOut().getSignal().getValue();
      }
    }
    return 0;
  }
 private:
  HomingSequence homingSequence;
  Move move;
  Wait pause;
  double angle, diff;
  SafetySystem& ss;
  ControlSystem& cs;
  SMCSafetyProperties& sp;
};

#endif // MAINSEQUENCE_HPP_ 

#ifndef MAINSEQUENCE_HPP_
#define MAINSEQUENCE_HPP_

#include <eeros/sequencer/Sequencer.hpp>
#include <eeros/sequencer/Sequence.hpp>
#include <eeros/sequencer/Step.hpp>
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
	}
	double pos;
	ControlSystem& cs;
};

class MainSequence : public Sequence {
public:
	MainSequence(std::string name, Sequencer& seq, SafetySystem& safetySys, SMCSafetyProperties& safetyProp, ControlSystem& cs, double angle) : 
					Sequence(name, seq), safetySys(safetySys), safetyProp(safetyProp), angle(angle), controlSys(cs), move("move", this, cs) {
		log.info() << "Sequence created: " << name;
	}
	int action() {
		while(safetySys.getCurrentLevel() < safetyProp.slMoving);
	
		angle = 0;
		while (Sequencer::running) {
			angle += 6.28 / 10;
			move(angle);
			sleep(1);
			log.info() << "enc =  " << controlSys.enc.getOut().getSignal().getValue();
		}
	}
private:
	Move move;
	double angle;
	SafetySystem& safetySys;
	ControlSystem& controlSys;
	SMCSafetyProperties& safetyProp;
};

#endif // MAINSEQUENCE_HPP_ 

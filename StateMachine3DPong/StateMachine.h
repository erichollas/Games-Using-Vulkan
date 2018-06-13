/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		StateMachine.h																*
* DETAILS:	This file defines the StateMachine class. The state machine will be a		*
*				memeber variable in each of the character classes and manage each		*
*				of the characters states.												*
*																						*
*****************************************************************************************/


#pragma once
#include "../../StateMachine3DPong/States/BaseState.h"

template <class character_type>
class StateMachine {
public:
	StateMachine(character_type * character) {
		p_owner = character;
		p_currState = NULL;
	}

	void setCurrentState(state<character_type> * st) {
		p_prevState = p_currState;
		p_currState = st;
	}
	void runSM() const {
		if (p_currState != NULL) {
			p_currState->execute(p_owner);
		}
	}

	void handleMessage(message &msg) {
		p_currState->onMessage(p_owner, msg);
	}

	state<character_type>* getCurrentState() const {
		return p_currState;
	}
	state<character_type>* getPreviousState() const {
		return p_prevState;
	}


private:
	character_type * p_owner;

	state<character_type> * p_currState;
	state<character_type> * p_prevState;
};
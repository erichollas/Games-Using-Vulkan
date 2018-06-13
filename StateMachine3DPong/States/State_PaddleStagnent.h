/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PaddleStagnentState.h														*
* DETAILS:	This file defines the state in which the paddle character stays				*
*				stationary.																*
*																						*
*****************************************************************************************/


#ifndef STATE_PADDLESTAGNENT
#define STATE_PADDLESTAGNENT


#include "stdafx.h"
#include "../../StateMachine3DPong/States/BaseState.h"
#include "Character_Paddle.h"
#include "PhysicsCalculator.h"


class State_PaddleStagnent : public state<Character_Paddle> {
public:
	void execute(Character_Paddle * p_character) override {
		//do nothing
	}
	/*
	* Function: onMessage
	*
	* Paramters: Character_Paddle * p_character
	*			 message &msg
	*
	* Return Type: void
	*
	* Description: Processes the event that a message was sent to this character
	*				for this character the viable messages are collision_check,
	*				move_up, move_down, and ai_signal
	*				Collision check determines if there was a collision, if so
	*				responds to sender that there was a collision, otherwise does nothing
	*
	*/
	void onMessage(Character_Paddle * p_character, message &msg) override {
		float dist;
		bool isCollision;
		switch (msg.letter) {
		case collision_check:
			//calls PHYSICS, the physics calculator to determine if there was a collision
			//      dist is passed by reference, it will be sent in the response if there 
			//		was a collision
			isCollision = PHYSICS->isCollision(dist,
				msg.post_script.position,
				msg.post_script.radius,
				p_character->getLocation().z,
				p_character->getRadius());
			if (isCollision) {
				//calls DISPATCH, the messanger instance, to send the response message
				DISPATCH->collect(collision_happened, 2, p_character->getID(), msg.post_script.position, p_character->getRadius(), dist);
			}
			break;
		case move_up:
			//changes state
			p_character->moveUp();
			break;
		case move_down:
			//changes state
			p_character->moveDown();
			break;
		case ai_signal:
			//changes state
			p_character->getSmart();
			break;
		}
	}
	/*
	* Function: instance
	*
	*
	* Description: Retruns a static instance of the class
	*
	*/
	static State_PaddleStagnent* instance();
};

#endif /*STATE_PADDLESTAGNENT*/

State_PaddleStagnent* State_PaddleStagnent::instance() {
	static State_PaddleStagnent inst;
	return &inst;
}

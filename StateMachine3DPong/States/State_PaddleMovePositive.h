/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PaddleMovePositiveState.h													*
* DETAILS:	This file defines the state in which the paddle character moves in the		*
*				positive direction.														*
*																						*
*****************************************************************************************/


#ifndef STATE_PADDLEMOVEPOSITIVE
#define STATE_PADDLEMOVEPOSITIVE

#include "stdafx.h"
#include "../../StateMachine3DPong/States/BaseState.h"
#include "Character_Paddle.h"
#include "PhysicsCalculator.h"


class State_PaddleMovePositive : public state<Character_Paddle> {
public:
	/*
	* Function: execute
	*
	* Paramters: Character_Paddle * p_character
	*
	* Return Type: void
	*
	* Description: This function executes one cycle for the state that the
	*				paddle moves in the positive direction, only moves once
	*				and then changes states
	*				Note that the uniform matrix data for the paddle is updated here
	*
	*/
	void execute(Character_Paddle * p_character) override {
		glm::vec3 currLocation = p_character->getLocation();
		float z_movement = 0.030f;
		if ((currLocation.z + z_movement) <= (0.8f - p_character->getRadius())) {
			glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3{ 0.0f, 0.0f, (currLocation.z + z_movement) });
			p_character->setUniformMatrix(matrix);
			p_character->setLocation(glm::vec3{ currLocation.x, currLocation.y, (currLocation.z + z_movement) });
		}
		p_character->stopMoving();
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
	*				move_up, and move_down
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
		}
	}
	/*
	* Function: instance
	*
	*
	* Description: Retruns a static instance of the class
	*
	*/
	static State_PaddleMovePositive* instance();
};

#endif /*STATEPADDLEMOVEPOSITIVE*/

State_PaddleMovePositive* State_PaddleMovePositive::instance() {
	static State_PaddleMovePositive inst;
	return &inst;
}

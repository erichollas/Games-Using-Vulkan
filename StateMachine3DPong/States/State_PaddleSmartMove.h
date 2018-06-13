/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PaddleSmartMoveState.h														*
* DETAILS:	This file defines the state in which the paddle character is moving			*
*				according to the calculations of the AI_ENGINE, defined in AIengine.h	*
*																						*
*****************************************************************************************/


#ifndef STATE_PADDLESMARTMOVE
#define STATE_PADDLESMARTMOVE


#include "stdafx.h"
#include "../../StateMachine3DPong/States/BaseState.h"
#include "Character_Paddle.h"
#include "PhysicsCalculator.h"
#include "AIengine.h"


class State_PaddleSmartMove : public state<Character_Paddle> {
public:
	/*
	* Function: execute
	*
	* Paramters: Character_Paddle * p_character
	*
	* Return Type: void
	*
	* Description: Calls AI_ENGINE, the instance of the AIengine class, to get the target position
	*				then determines if the paddle should stop moving, move up or move down
	*				Note that the uniform matrix for the paddle is calculated and set here
	*
	*
	*/
	void execute(Character_Paddle * p_character) override {
		float zPosition = p_character->getLocation().z;
		float target = AI_ENGINE->getTargetPosition();

		if (zPosition - target < Constants::ACCURACY_EPSILON &&
			zPosition - target > -Constants::ACCURACY_EPSILON) {
			p_character->stopMoving();
		}
		else if (zPosition < target) {
			glm::vec3 currLocation = p_character->getLocation();
			float z_movement = 0.0005f;
			if ((currLocation.z + z_movement) <= (0.8f - p_character->getRadius())) {
				glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3{ 0.0f, 0.0f, (currLocation.z + z_movement) });
				p_character->setUniformMatrix(matrix);
				p_character->setLocation(glm::vec3{ currLocation.x, currLocation.y, (currLocation.z + z_movement) });
			}
		}
		else {
			glm::vec3 currLocation = p_character->getLocation();
			float z_movement = -0.0005f;
			if ((currLocation.z + z_movement) >= (-0.8f + p_character->getRadius())) {
				glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3{ 0.0f, 0.0f, (currLocation.z + z_movement) });
				p_character->setUniformMatrix(matrix);
				p_character->setLocation(glm::vec3{ currLocation.x, currLocation.y, (currLocation.z + z_movement) });
			}
		}
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
	*				for this character the only viable message is collision_check.
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
				DISPATCH->collect(collision_happened, 2, 1, msg.post_script.position, p_character->getRadius(), dist);
			}
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
	static State_PaddleSmartMove* instance();
};

#endif /*STATE_PADDLESMARTMOVE*/

State_PaddleSmartMove* State_PaddleSmartMove::instance() {
	static State_PaddleSmartMove inst;
	return &inst;
}

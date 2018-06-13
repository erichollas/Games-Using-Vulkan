/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BallTowardsComputerState.h													*
* DETAILS:	This file defines the State instance class for the situation when the ball	*
*				is moving towards the computer paddle's side.							*
*																						*
*****************************************************************************************/


#ifndef STATE_BALLTOWARDSCOMPUTER
#define STATE_BALLTOWARDSCOMPUTER

#include "stdafx.h"
#include "../../StateMachine3DPong/States/BaseState.h"
#include "Character_Ball.h"
#include "PhysicsCalculator.h"


class State_BallTowardsComputer : public state<Character_Ball> {
public:
	/*
	* Function: execute
	*
	* Paramters: Character_Ball * p_ball
	*
	* Return Type: void
	*
	* Description: This function executes one cycle for the state that the
	*				ball is moving towards the computer
	*
	*/
	void execute(Character_Ball * p_ball) override {
		glm::vec3 location = p_ball->getLocation();
		//calls PHYSICS, the physics calculator, to determine the next uniform matrix,
		//   location will be passed by reference
		p_ball->setUniformMatrix(PHYSICS->ballRoll(p_ball->getTrajectoryAngle(),
			p_ball->getTrajectorySpeed(),
			location,
			p_ball->isReflecting(),
			p_ball->getUniformMatrix()));
		//updates location
		p_ball->setLocation(location);

		//checks for each case to send a message to a paddle for a collision check or if it needs to reflect
		float radius = p_ball->getRadius();
		if (!(location.z < (0.8f - radius) && location.z >(-0.8f + radius))) {
			p_ball->reflect();
		}
		if (location.x >= 0.9f - p_ball->getRadius()) {
			DISPATCH->collect(collision_check, 1, p_ball->getID(), location.z, p_ball->getRadius(), 0.0f);
		}
		if (location.x >= 1.0f - p_ball->getRadius()) {
			p_ball->resetBall();
			DISPATCH->collect(point_scored, 3, 2);
		}
	}
	/*
	* Function: exit
	*
	* Paramters: Character_Ball * p_ball
	*
	* Return Type: void
	*
	* Description: re-init's the uniform matrix to the position it is in and then changes the state of the ball
	*				to move to the user
	*
	*/
	void exit(Character_Ball * p_ball) {
		p_ball->setUniformMatrix(glm::translate(glm::mat4(1.0f), p_ball->getLocation()));
		p_ball->aimAtUser();
	}
	/*
	* Function: onMessage
	*
	* Paramters: Character_Ball * p_ball,
	*			 message &msg
	*
	* Return Type: void
	*
	* Description: Processes the event that a message was sent to this character
	*				for this character there only viable message it should receive
	*				is that a collision happened
	*
	*/
	void onMessage(Character_Ball * p_ball, message &msg) override {
		if (msg.letter == collision_happened) {
			float spd, dgr;
			//calls PHYSICS, the physics calculator to determine the trajectory to reflect in,
			//   dgr and spd are degrees and speed and are passed by reference
			PHYSICS->determineHitResultant(dgr, spd, msg.post_script.distance, msg.post_script.radius);

			//+ 180 since this ball is in the towards computer state
			dgr += 180.0f;

			p_ball->setTrajectoryInfo(spd, dgr);
			exit(p_ball);
		}
	}
	/*
	* Function: instance
	*
	*
	* Description: Retruns a static instance of the class
	*
	*/
	static State_BallTowardsComputer* instance();
};


#endif /*STATE_BALLTOWARDSCOMPUTER*/

State_BallTowardsComputer* State_BallTowardsComputer::instance() {
	static State_BallTowardsComputer inst;
	return &inst;
}
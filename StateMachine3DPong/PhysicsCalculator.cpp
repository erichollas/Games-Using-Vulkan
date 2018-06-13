/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PhysicsCalculator.cpp														*
*																						*
* Correponds to PhysicsCalculator.h (see for overall description)						*
*																						*
*****************************************************************************************/


#include "stdafx.h"
#include "PhysicsCalculator.h"

/*
* Function: instance
*
* Description: Retruns a static instance of the class
*
*/
PhysicsCalculator* PhysicsCalculator::instance() {
	static PhysicsCalculator inst;
	return &inst;
}

/*
* Function: isCollision
*
* Paramters:	float &distance,	passed by reference
*				float positionBall, 
*				float radiusBall, 
*				float positionBlock, 
*				float radiusBlock
*
* Return Type: bool
*
* Description: Calculates if there was a colision using the paramters
*				returns true if there was, otherwise returns flase
*				also, if true, distance will be the distance of the
*				point of collision and the center of the paddle, which
*				will be used in the ball's next trajectory
*
*/
bool PhysicsCalculator::isCollision(float &distance, 
									float positionBall, 
									float radiusBall, 
									float positionBlock, 
									float radiusBlock) {
	if (positionBall < (positionBlock + radiusBlock) && positionBall >(positionBlock - radiusBlock)) {
		distance = abs(abs(positionBall) - abs(positionBlock));
		if (positionBall < positionBlock) {
			distance = -distance;
		}
		return true;
	}
	else if (positionBall <= (positionBlock + radiusBlock + radiusBall) && positionBall >= (positionBlock - radiusBlock - radiusBall)) {
		//set to 10.0f for farthest distance possible
		distance = 10.0f;
		if (positionBall < positionBlock) {
			distance = -distance;
		}
		return true;
	}
	return false;
}
/*
* Function: determineHitResultant
*
* Paramters:	float &degrees,		passed by reference
*				float &speed,		passed by reference
*				float distance, 
*				float blockRadius
*
* Return Type: void
*
* Description: Determines the trajectory of the ball from the distance and blockRadius
*				parameters. Stores the results in the degrees and speed parameters.
*
*/
void PhysicsCalculator::determineHitResultant(float &degrees, float &speed, float distance, float blockRadius) {
	//calculates the interval for more intense trajectory the farther from the paddle
	float interval = blockRadius / 7.0f;
	uint8_t count = 1;
	float absv_dist = abs(distance);

	//determines which interval the ball hit
	while (!(absv_dist < (count * interval)) && count < 6) {
		count++;
	}

	//sets the results based on the interval
	speed = ((float)(count) * 0.0001f) + 0.0005f;
	degrees = (float)(count - 1) * 7.5f;

	//reflects the direction if negative
	if (distance < 0.0f) {
		degrees = -degrees;
	}
}
/*
* Function: ballRoll
*
* Paramters:	float degrees, 
*				float speed, 
*				glm::vec3 &location,	passed by reference
*				bool isReflecting,
*				glm::mat4 oldmatrix
*
* Return Type: glm::mat4
*
* Description: Calculates and returns the uniform matrix for the ball's next position
*				and orientation.
*				Location is passed by reference to use to update the location of the ball
*				which will be used to determine collisions and future ball rolls.
*
*/
glm::mat4 PhysicsCalculator::ballRoll(float degrees, 
									  float speed, 
									  glm::vec3 &location, 
									  bool isReflecting, 
									  glm::mat4 oldmatrix) {
	//determines the change in location
	float x_component = speed * cos(glm::radians(degrees));
	float z_component = speed * sin(glm::radians(degrees));
	//makes perpendicular so the rotation of the ball is counter-clockwise
	float axis = 270.0f + degrees;

	glm::mat4 rotationMatrix;

	//changes the axis and z_component if the ball is on a reflecting path
	if (isReflecting) {
		z_component = - z_component;
		axis = 270.0f - degrees;
	}

	//updates the location
	location.x += x_component;
	location.z += z_component;

	//calculates and returns the matrix
	glm::mat4 translationMatrix = glm::mat4{ 0.0f, 0.0f, 0.0f, 0.0f,
											 0.0f, 0.0f, 0.0f, 0.0f,
											 0.0f, 0.0f, 0.0f, 0.0f,
											 x_component, 0.0f, z_component, 0.0f
											};
	translationMatrix = oldmatrix + translationMatrix;
	axis = glm::radians(axis);
	rotationMatrix = glm::rotate(glm::mat4(1.0f), speed * glm::radians(720.0f), glm::vec3{ cos(axis), 0.0f, sin(axis) });
	return translationMatrix * rotationMatrix;
}
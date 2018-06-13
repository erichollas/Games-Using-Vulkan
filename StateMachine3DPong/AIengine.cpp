/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		AIengine.cpp																*
*																						*
* Correponds to AIengine.h (see for overall description)								*
*																						*
*****************************************************************************************/


#include "stdafx.h"
#include "AIengine.h"


/*
* Function: instance
*
* 
* Description: Retruns a static instance of the class
*
*/
AIengine* AIengine::instance() {
	static AIengine inst;
	return &inst;
}
/*
* Function: calcTargetPosition
*
* Paramters: float collisionPosition, 
*			 float degrees, 
*			 float speed
*
* Return Type: void
*
* Description: Calculates the balls future position based on the parameters
*				stores the calculation in the member variable, targetPosition
*
*/
void AIengine::calcTargetPosition(float collisionPosition, float degrees, float speed) {
	targetPosition = collisionPosition;

	if (degrees == 0.0f) {
		return;
	}

	bool hasHitWall = false;
	float x_position = -0.9;
	while (x_position < 0.9) {
		float x_delta, y_delta;
		x_delta = speed * cos(glm::radians(degrees));
		y_delta = speed * sin(glm::radians(degrees));
		if (hasHitWall) {
			y_delta = -y_delta;
		}
		x_position += x_delta;
		targetPosition += y_delta;

		if (targetPosition <= -0.8 || targetPosition >= 0.8) {
			hasHitWall = !hasHitWall;
		}
	}
}
/*
* Function: getTargetPosition
*
*
* Description: simple getter method
*
*/
float AIengine::getTargetPosition() const {
	return targetPosition;
}
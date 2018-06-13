/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PhysicsCalculator.h															*
* DETAILS:	This file defines the instance class that contains the functions that are	*
*				used to calculate the game physics, namely the ball collision,			*
*				trajectory, and the ball's uniform matrix.								*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"

class PhysicsCalculator {
public:
	static PhysicsCalculator* instance();

	bool isCollision(float &distance, float positionBall, float radiusBall, float positionBlock, float radiusBlock);

	void determineHitResultant(float &degrees, float &speed, float distance, float blockRadius);

	glm::mat4 ballRoll(float degrees, float speed, glm::vec3 &location, bool isReflecting, glm::mat4 oldMatrix);
};

#define PHYSICS PhysicsCalculator::instance()
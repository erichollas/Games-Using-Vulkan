/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		AIengine.h																	*
* DETAILS:	This file defines the ai engine. At the end of the file AI_ENGINE is		*
*				defined using a macro. This will serve as the instance throughout		*
*				the rest of the program. The class stores a target point that can		*
*				be returned and has another function that calculates a future point		*
*				to be used for the computer controlled paddle.							*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"

class AIengine {
public:
	static AIengine* instance();

	void calcTargetPosition(float collisionPosition, float degrees, float speed);

	float getTargetPosition() const;

private:
	float targetPosition = 0.0f;
};

#define AI_ENGINE AIengine::instance()
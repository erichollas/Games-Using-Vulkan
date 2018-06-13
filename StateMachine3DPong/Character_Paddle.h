/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PaddleCharacter.h															*
* DETAILS:	This file the paddle character. The paddle character builds off of the		*
*				BaseCharacter class. It also defines the functions used to change		*
*				states via the p_StateMachine member variable.							*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "BaseCharacter.h"
#include "StateMachine.h"

//supported colors are red, green, or blue
class Character_Paddle : public BaseCharacter {
public:
	Character_Paddle(float radius, glm::vec3 initPosition);
	~Character_Paddle();

	void initMesh(Constants::color shade) override;
	bool runCycle() override;
	void handleMessages() override;

	void stopMoving();
	void moveUp();
	void moveDown();
	void getSmart();

private:
	StateMachine<Character_Paddle> * p_StateMachine;
};
/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		PaddleCharacter.cpp															*
*																						*
* Correponds to PaddleCharacter.h (see for overall description)							*
*																						*
*****************************************************************************************/

#include "stdafx.h"
#include "Character_Paddle.h"
#include "../../StateMachine3DPong/States/State_PaddleStagnent.h"
#include "../../StateMachine3DPong/States/State_PaddleMoveNegative.h"
#include "../../StateMachine3DPong/States/State_PaddleMovePositive.h"
#include "../../StateMachine3DPong/States/State_PaddleSmartMove.h"

/*
* Constructor
*
* Parameters: std::string title
*			  float radius
*			  glm::vec3 initPosition
*
* parameters are passed to BaseCharacter class
*
*/
Character_Paddle::Character_Paddle(float radius, glm::vec3 initPosition)
				: BaseCharacter(radius, initPosition) {
	p_StateMachine = new StateMachine<Character_Paddle>(this);
	p_StateMachine->setCurrentState(State_PaddleStagnent::instance());
}
/*
* Deconstructor
*/
Character_Paddle::~Character_Paddle() {}

/*
* Function: initMesh
*
* Paramters: Paramters: Constants::color
*
* Return Type: void
*
* Description: initializes the mesh data for the character
*				the only viable parameters are red, green, and blue
*				otherwise blue will be the default value
*				the function is set to vary the shades of the color
*				to make the character appear 3 dimensional
*
*/
void Character_Paddle::initMesh(Constants::color shade) {
	glm::vec3 middleShade = Constants::color_pallete[shade];
	glm::vec3 darkerShade;
	glm::vec3 lighterShade;
	switch (shade) {
	case Constants::red:
		darkerShade = glm::vec3{ 0.6f, 0.0f, 0.0f };
		lighterShade = glm::vec3{ 1.0f, 0.2f, 0.6f };
		break;
	case Constants::green:
		darkerShade = glm::vec3{ 0.0f, 0.3f, 0.0f };
		lighterShade = glm::vec3{ 0.1f, 0.9f, 0.4f };
		break;
	case Constants::blue:
	default:
		darkerShade = glm::vec3{ 0.0f, 0.0f, 0.4f };
		lighterShade = glm::vec3{ 0.0f, 0.4f, 1.0f };
		break;
	}
	glm::vec3 location = getLocation();
	std::vector<Geometry::Vertex> verts = {
		{ { location.x,			0.0f,  getRadius() }, lighterShade },	//index 0
		{ { location.x,			0.1f,  getRadius() }, darkerShade },	//index 1
		{ { location.x - 0.1f,	0.1f,  getRadius() }, lighterShade },	//index 2
		{ { location.x - 0.1f,	0.0f,  getRadius() }, middleShade },	//index 3
		{ { location.x,			0.1f, -getRadius() }, lighterShade },	//index 4
		{ { location.x,			0.0f, -getRadius() }, middleShade },	//index 5
		{ { location.x - 0.1f,	0.1f, -getRadius() }, darkerShade },	//index 6
		{ { location.x - 0.1f,	0.0f, -getRadius() }, lighterShade }	//index 7
	};
	std::vector<uint32_t> inds = {
		//front
		0, 3, 2,
		2, 1, 0,

		//left
		0, 1, 4,
		4, 5, 0,

		//top
		1, 2, 6,
		6, 4, 1,

		//right
		7, 6, 2,
		2, 3, 7
	};
	setGeometry(verts, inds);
}
/*
* Function: runCycle
*
* Paramters: none
*
* Return Type: bool
*
* Description: processes messages recevied if there are any, other runs
*				once on the state it's currently in
*				For this class this function should always return true
*
*/
bool Character_Paddle::runCycle() {
	if (mailReceived()) {
		handleMessages();
	}
	p_StateMachine->runSM();

	return true;
}
/*
* Function: handleMessages
*
* Paramters: none
*
* Return Type: void
*
* Description: preocesses the messages if there are any for the character
*
*/
void Character_Paddle::handleMessages() {
	std::deque<message> mail = getMail();
	while (!(mail.empty())) {
		p_StateMachine->handleMessage(mail.front());
		mail.pop_front();
	}
	readMessages();
}

/*
* The rest of the methods change the state of this state machine character
*     to the various other states.
*/
void Character_Paddle::stopMoving() {
	p_StateMachine->setCurrentState(State_PaddleStagnent::instance());
}
void Character_Paddle::moveUp() {
	p_StateMachine->setCurrentState(State_PaddleMoveNegative::instance());
}
void Character_Paddle::moveDown() {
	p_StateMachine->setCurrentState(State_PaddleMovePositive::instance());
}
void Character_Paddle::getSmart() {
	p_StateMachine->setCurrentState(State_PaddleSmartMove::instance());
}
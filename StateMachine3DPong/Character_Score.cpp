/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		ScoreCharacter.cpp															*
*																						*
* Correponds to ScoreCharacter.h (see for overall description)							*
*																						*
*****************************************************************************************/


#include "stdafx.h"
#include "Character_Score.h"
#include "../../StateMachine3DPong/States/State_Score.h"

/*
*Constructor for the locally defined struct score
*/
Character_Score::score::score() {
}
/* belongs to the locally defined struct score
* Function: setColor
*
* Paramters: bool isUser
*
* Return Type: void
*
* Description: determines the different color for if the Scorecharacter is
*				a user score or the computer score
*
*/
void Character_Score::score::setColor(bool isUser) {
	if (isUser) {
		color = Constants::color_pallete[Constants::blue];
	}
	else {
		color = Constants::color_pallete[Constants::red];
	}
}
/* belongs to the locally defined struct score
* Function: reset
*
* Paramters: none
*
* Return Type: void
*
* Description: init's or re-init's the member variables
*
*/
void Character_Score::score::reset() {
		value = 0;

		zero temp;
		temp.setColor(Constants::color_pallete[Constants::gray]);
		temp.initInfo();

		vertices = temp.vertices;
		indices = temp.indices;
}
/* belongs to the locally defined struct score
* Function: inc
*
* Paramters: none
*
* Return Type: bool
*
* Description: updates the score and updates the vertex and index data
*				using one of the objects defined in Numbers.h,
*				increases value and returns false if the score has gone
*				past 9 otherwise returns true
*
*/
bool Character_Score::score::inc() {
	if (value >= 9) {
		return false;
	}
	else {
		//an if-else must be here instead of a switch beacuse of the need
		//   for locally defined variables, which are not allowed in a switch
		if (value == 0) {
			one temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 1) {
			two temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 2) {
			three temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 3) {
			four temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 4) {
			five temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 5) {
			six temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 6) {
			seven temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 7) {
			eight temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
		else if (value == 8) {
			nine temp;
			temp.setColor(color);
			temp.initInfo();

			vertices = temp.vertices;
			indices = temp.indices;
		}
	}

	value++;

	return true;
}




/*
* Constructor
*
* Parameters: glm::vec3 initPosition
*
* there is no radius, so just pass 0.0f for the radius
*
*/
Character_Score::Character_Score(glm::vec3 initLocation)
			   : BaseCharacter(0.0f, initLocation) {
	p_StateMachine = new StateMachine<Character_Score>(this);
	p_StateMachine->setCurrentState(State_Score::instance());

	p_points = new score();

	//determines if this is user or computer score
	if (initLocation.x > 1.0f) {
		p_points->setColor(false);
	}
	else {
		p_points->setColor(true);
	}

	p_points->reset();
	setUniformMatrix(glm::translate(glm::mat4(1.0f), initLocation));
}
/*
* Deconstructor
*/
Character_Score::~Character_Score() {
}

/*
* Function: initMesh
*
* Paramters: Constants::color
*
* Return Type: void
*
* Description: Ignores the parameter, updates the vertex and index
*				info from the p_points member variable.
*				This will be called several times, updating the 
*				changes to vertex and index info is handled in messages
*				and the struct score member functions defined above.
*
*/
void Character_Score::initMesh(Constants::color shade) {
	setGeometry(p_points->vertices, p_points->indices);
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
*				Returns true if there has been a score change, otherwise returns false
*
*/
bool Character_Score::runCycle() {
	bool dontUpdate = true;
	if (mailReceived()) {
		handleMessages();
		dontUpdate = false;
	}
	p_StateMachine->runSM();
	return dontUpdate;
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
void Character_Score::handleMessages() {
	std::deque<message> mail = getMail();
	while (!(mail.empty())) {
		p_StateMachine->handleMessage(mail.front());
		mail.pop_front();
	}
	readMessages();
}

/*
* Function: ScoreReset
*
* Paramters: none
*
* Return Type: void
*
* Description: re-init's the p_points and vertex and index info
*
*/
void Character_Score::ScoreReset() {
	p_points->reset();
	initMesh();
}
/*
* Function: ScorePoint
*
* Paramters: none
*
* Return Type: bool
*
* Description: changes the vertex and index info for this character by calling
*				the member functions for the struct score, returns true to signal
*				that the game is over and both scores need to be reset, otherwise returns false
*
*/
bool Character_Score::ScorePoint() {
	bool needsReset = p_points->inc();

	initMesh();

	return needsReset;
}
/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		CharacterManager.cpp														*
*																						*
* Correponds to CharacterManager.h (see for overall description)						*
*																						*
*****************************************************************************************/

#include "stdafx.h"
#include "CharacterManager.h"
#include "ScenicObjects.h"
#include "Character_Ball.h"
#include "Character_Paddle.h"

/*
* Constructor
*/
CharacterManager::CharacterManager() {
	//scenic objects are defined in GeometryManager()
	p_Offsets = new Geometry::GeometryManager();
	initScenicCharacters();
	//initialize uniform matrices with the scenic matrices
	for (uint32_t i = 0; i < p_Offsets->getNumOfObjects(); i++) {
		uniformMatrices.push_back(glm::mat4(1.0f));
	}
}
/*
* DeConstructor
*/
CharacterManager::~CharacterManager() {
	uniformMatrices.~vector();
	p_Offsets->~GeometryManager();
}

/*
* Function: AddCharacter
*
* Paramters: BaseCharacter * newCharacter
*
* Return Type: void
*
* Description: takes the parameter and adds its vertex and index info
*				to the p_Offsets member variable, adds its uniform matrix
*				to the uniformMatrices, and adds the character to the MasterList
*
*/
void CharacterManager::AddCharacter(BaseCharacter * newCharacter) {
	p_Offsets->addObject(newCharacter->getVertices(), newCharacter->getIndices());
	MasterList.insert({ newCharacter->getID(), newCharacter });
	uniformMatrices.push_back(newCharacter->getUniformMatrix());
}
/*
* Function: DeleteCharacter
*
* Paramters: BaseCharacter * newCharacter
*
* Return Type: void
*
* Description: uses the parameter to delete that character from the list with a matching id
*				note that id's should be unique
*
*/
void CharacterManager::DeleteCharacter(BaseCharacter * character) {
	MasterList.erase(character->getID());
	character->~BaseCharacter();
}

/*
* Function: updateOffsets
*
* Paramters: none
*
* Return Type: void
*
* Description: The score characters will be the last characters in the MasterList
*				this needs to be ensured in the initCharacters function in the Window class.
*				The score characters vertex and index info will change, so this function 
*				deletes from p_Offsets and updates p_Offsets with the new vertex and index info.
*
*/
void CharacterManager::updateOffsets() {
	p_Offsets->deleteLast();
	p_Offsets->deleteLast();

	int index = MasterList.size();

	p_Offsets->addObject(MasterList[index - 2]->getVertices(), MasterList[index - 2]->getIndices());
	p_Offsets->addObject(MasterList[index - 1]->getVertices(), MasterList[index - 1]->getIndices());
}
/*
* Function: runFSMs
*
* Paramters: none
*
* Return Type: void
*
* Description: This function will run the states via the runCycle functions of all the characters in
*				the MasterList. It also determines if the score characters vertex and index info changed
*				and updates them accordingly. The function finishes with updating the uniformMatrices
*				after all the events have been calculated this cycle.
*
*/
bool CharacterManager::runFSMs() {
	bool updateOffsets = false;
	for (uint32_t i = 0; i < MasterList.size(); i++) {
		bool flag = MasterList[i]->runCycle();
		if (!flag) {
			updateOffsets = true;
		}
	}
	updateMatrices();

	return updateOffsets;
}
/*
* Function: userInput
*
* Paramters: bool up, 
*			 bool down, 
*			 int user_id
*
* Return Type: void
*
* Description: processes the user input from the window class and sends 
*				a message via DISPATCH, the messanger instance, to the
*				user controlled paddle
*
*/
void CharacterManager::userInput(bool up, bool down, int user_id) {
	if (up) {
		DISPATCH->collect(move_up, user_id, -1);
	}
	if (down) {
		DISPATCH->collect(move_down, user_id, -1);
	}
}

//getter method
std::vector<glm::mat4> CharacterManager::getUniformMatrices() const {
	return uniformMatrices;
}
//getter method
Geometry::GeometryManager* CharacterManager::getOffsets() const {
	return p_Offsets;
}


/*
* Function: initScenicCharacters
*
* Paramters: none
*
* Return Type: void
*
* Description: Initializes the offset manager with the scenic characters
*
*/
void CharacterManager::initScenicCharacters() {
	//add the scenic characters
	homeGoalLine scenic1;
	p_Offsets->addObject(scenic1.vertices, scenic1.indices);
	midFieldLine scenic2;
	p_Offsets->addObject(scenic2.vertices, scenic2.indices);
	awayGoalLine scenic3;
	p_Offsets->addObject(scenic3.vertices, scenic3.indices);
	homeField scenic4;
	p_Offsets->addObject(scenic4.vertices, scenic4.indices);
	awayField scenic5;
	p_Offsets->addObject(scenic5.vertices, scenic5.indices);
	farBarrier scenic6;
	p_Offsets->addObject(scenic6.vertices, scenic6.indices);
	nearBarrier scenic7;
	p_Offsets->addObject(scenic7.vertices, scenic7.indices);
}
/*
* Function: updateMatrices
*
* Paramters: none
*
* Return Type: void
*
* Description: Gets the updated matrices from the characters after the cycle has run.
*				Make sure to call this before returning the uniformMatrices class
*				member or the matrices could be out of date
*
*/
void CharacterManager::updateMatrices() {
	uniformMatrices.clear();
	//this first while loop is for the scenic objects uniform matrices
	for (uint32_t i = 0; i < (p_Offsets->getNumOfObjects() - MasterList.size()); i++) {
		uniformMatrices.push_back(glm::mat4(1.0f));
	}
	for (uint32_t i = 0; i < MasterList.size(); i++) {
		uniformMatrices.push_back(MasterList[i]->getUniformMatrix());
		
	}
}
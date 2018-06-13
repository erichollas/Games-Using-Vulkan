/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BaseCharacter.cpp															*
*																						*
* Correponds to BaseCharacter.h (see for overall description)							*
*																						*
*****************************************************************************************/


#include "stdafx.h"
#include "BaseCharacter.h"

//used to calculate the character's id
int BaseCharacter::next_available_id = 0;

/*
* Constructor
* parameters are passed to the base class
*/
BaseCharacter::BaseCharacter(float rad, glm::vec3 initLocation) {
	id = next_available_id++;

	radius = rad;
	currLocation = initLocation;
	uniformMatrix = glm::mat4(1.0f);
}
BaseCharacter::~BaseCharacter() {
	next_available_id--;
}

//setter function
void BaseCharacter::setUniformMatrix(glm::mat4 newMatrix) {
	uniformMatrix = newMatrix;
}
//setter function
void BaseCharacter::setLocation(glm::vec3 newLocation) {
	currLocation = newLocation;
}
//setter function
void BaseCharacter::setGeometry(std::vector<Geometry::Vertex> verts, std::vector<uint32_t> inds) {
	vertices = verts;
	indices = inds;
}
/*
* Function: mailReceived
*
* Paramters: none
*
* Return Type: bool
*
* Description: gets all the messages from DISPATCH, the messanger instance, 
*				and returns true if there was a message received
*
*/
bool BaseCharacter::mailReceived() {
	return DISPATCH->distribute(id, mailbox);
}
/*
* Function: readMessages
*
* Paramters: none
*
* Return Type: void
*
* Description: clears the mailbox deque of received messages
*				only call this function after the messages have been processed
*
*/
void BaseCharacter::readMessages() {
	mailbox.clear();
}

/*
* The rest of the functions in this file are generic getter functions
*/
int BaseCharacter::getID() const {
	return id;
}
float BaseCharacter::getRadius() const {
	return radius;
}
std::vector<Geometry::Vertex> BaseCharacter::getVertices() const {
	return vertices;
}
std::vector<uint32_t> BaseCharacter::getIndices() const {
	return indices;
}
glm::vec3 BaseCharacter::getLocation() const {
	return currLocation;
}
glm::mat4 BaseCharacter::getUniformMatrix() const {
	return uniformMatrix;
}
std::deque<message> BaseCharacter::getMail() const {
	return mailbox;
}
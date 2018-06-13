/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Messanger.cpp																*
*																						*
* Correponds to Messanger.h (see for overall description)								*
*																						*
*****************************************************************************************/


#include "stdafx.h"
#include "Messanger.h"


/*
* Function: instance
*
*
* Description: Retruns a static instance of the class
*
*/
Messanger* Messanger::instance() {
	static Messanger inst;
	return &inst;
}

/*
* Function: collect
*
* This and the next function are overloaded, the second version adds the extra info to the messages
*
* Return Type: void
*
* Description: Takes the parameters, creates a message, and then adds it to the postOffice member
*				variable for distribution later.
*				Note that message_type is enum variable, possible values are defined in Messanger.h
*
*/
void Messanger::collect(message_type typ, int to, int from) {
	message msg;
	msg.letter = typ;
	msg.sender_id = from;
	msg.reciever_id = to;

	postOffice.push_back(msg);
}
void Messanger::collect(message_type typ, int to, int from, float position, float radius, float distance) {
	message msg;
	msg.letter = typ;
	msg.sender_id = from;
	msg.reciever_id = to;

	msg.post_script.position = position;
	msg.post_script.radius = radius;
	msg.post_script.distance = distance;

	postOffice.push_back(msg);
}
/*
* Function: distribute
*
* Paramters: int address_id, 
*			 std::deque<message> &mailbox
*
* Return Type: bool
*
* Description: Uses the address_id parameter to find all the messages (there may be more than one)
*				in the postOffice member variable and adds those messages to the mailbox variable,
*				which is passed by reference. Returns true if there was a message delerived to the
*				mailbox variable.
*				Note that once the message has been passed to the mailbox parameter it is deleted
*				from the postOffice member variable.
*
*/
bool Messanger::distribute(int address_id, std::deque<message> &mailbox) {
	bool isMail = false;

	uint32_t index = 0;
	while (index < postOffice.size()) {
		if (postOffice[index].reciever_id == address_id) {
			mailbox.push_back(postOffice[index]);
			postOffice.erase(postOffice.begin() + index);
			isMail = true;
		}
		else {
			index++;
		}
	}

	return isMail;
}
/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Messanger.h																	*
* DETAILS:	This file defines the Messanger. At the end of the file is the macro that	*
*				defines DISPATCH, the instance of the messanger used throughout the		*
*				program. DISPATCH will be in charge of collecting and distributing the	*
*				messages from machine to machine as well as the user input from the		*
*				Window class.															*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"


enum message_type {
	collision_check,
	collision_happened,
	move_up,
	move_down,
	ai_signal,
	point_scored,
	game_over
};

struct message {
	int sender_id;
	int reciever_id;

	message_type letter;

	struct extraInfo {
		float position;
		float radius;
		float distance;
	}post_script;
};


class Messanger {
public:
	static Messanger* instance();


	void collect(message_type msg, int to, int from);
	void collect(message_type typ, int to, int from, float position, float radius, float distance);

	bool distribute(int address_id, std::deque<message> &mailbox);


private:
	std::vector<message> postOffice;

};

#define DISPATCH Messanger::instance()
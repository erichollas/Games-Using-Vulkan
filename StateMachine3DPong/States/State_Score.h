/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		ScoreState.h																*
* DETAILS:	This file defines the the only state for the ScoreCharacters. This			*
*				character need only handle messages.									*
*																						*
*****************************************************************************************/


#ifndef STATE_SCORE_H
#define STATE_SCORE_H

#include "stdafx.h"
#include "../../StateMachine3DPong/States/BaseState.h"
#include "Character_Score.h"


class State_Score : public state<Character_Score> {
public:

	void execute(Character_Score * p_character) override {
		//do nothing
	}
	/*
	* Function: onMessage
	*
	* Paramters: Character_Score * p_ball,
	*			 message &msg
	*
	* Return Type: void
	*
	* Description: Processes the event that a message was sent to this character
	*				for this character the only viable messages it should receive
	*				are point_scored (one score has changed) or game_over (both scores need to change)
	*
	*				this function calls DISPATCH, which is the messanger instance
	*				also, the id's used to send the messages are known in the Window
	*				class function initCharacters
	*
	*/

	void onMessage(Character_Score * p_character, message &msg) override {
		if (msg.letter == point_scored) {
			bool needReset = p_character->ScorePoint();
			if (!needReset) {
				p_character->ScoreReset();
				if (p_character->getID() == 3) {
					DISPATCH->collect(game_over, 4, 3);
				}
				else {
					DISPATCH->collect(game_over, 3, 4);
				}
			}
		}
		else if (msg.letter == game_over) {
			p_character->ScoreReset();
		}
	}

	/*
	* Function: instance
	*
	*
	* Description: Retruns a static instance of the class
	*
	*/
	static State_Score* instance();
};


#endif /* STATE_SCORE_H */

State_Score* State_Score::instance() {
	static State_Score inst;
	return &inst;
}
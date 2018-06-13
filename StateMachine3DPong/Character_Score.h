/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		ScoreCharacter.h															*
* DETAILS:	This file defines the ScoreCharacter inherits from the BaseCharacter class. *
*				This character will be the score that is posted. One for the user and	*
*				another for the computer.												*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"
#include "Numbers.h"
#include "BaseCharacter.h"
#include "StateMachine.h"

class Character_Score : public BaseCharacter {
public:
	Character_Score(glm::vec3 initLocation);
	~Character_Score();

	void initMesh(Constants::color shade = Constants::gray) override;
	bool runCycle() override;
	void handleMessages() override;

	void ScoreReset();

	bool ScorePoint();

private:
	struct score {
		uint8_t value;
		glm::vec3 color;
		std::vector<Geometry::Vertex> vertices;
		std::vector<uint32_t> indices;

		score();

		void setColor(bool isUser);
		void reset();
		bool inc();
	};

	score * p_points;
	StateMachine<Character_Score> * p_StateMachine;
};
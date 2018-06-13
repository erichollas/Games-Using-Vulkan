/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BallCharacter.h																*
* DETAILS:	This file defines the ball object which inherits from the BaseCharacter 	*
*				class in the Geometry file. Note that initMesh() must be called for		*
*				for the vertices and indices to have any values.						*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "BaseCharacter.h"
#include "StateMachine.h"

class Character_Ball : public BaseCharacter {
public:
	Character_Ball(float radius, glm::vec3 initPosition);
	~Character_Ball();

	void initMesh(Constants::color shade = Constants::black) override;
	bool runCycle() override;
	void handleMessages() override;

	void reflect();
	void aimAtComputer();
	void aimAtUser();

	void setTrajectoryInfo(float speed, float degrees);
	void resetBall();

	float getTrajectorySpeed() const;
	float getTrajectoryAngle() const;
	glm::vec3 getRoationAxis() const;

	bool isReflecting() const {
		return heading.isReflecting;
	}

private:
	struct trajectory {
		glm::vec3 rotation_axis;
		float speed;
		float degrees;
		bool isReflecting;
	};

	trajectory heading;

	StateMachine<Character_Ball> * p_StateMachine;
};

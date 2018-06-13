/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BallCharacter.cpp															*
*																						*
* Correponds to BallCharacter.h (see for overall description)							*
*																						*
*****************************************************************************************/

#include "stdafx.h"
#include "Character_Ball.h"
#include "../../StateMachine3DPong/States/State_BallTowardsPlayer.h"
#include "../../StateMachine3DPong/States/State_BallTowardsComputer.h"

/*
* Constructor
*
* Parameters: float radius
*			  glm::vec3 initPosition
*
* parameters are passed to BaseCharacter class and initializes the member vectors
*
*/
Character_Ball::Character_Ball(float radius, glm::vec3 initPosition)
			  : BaseCharacter(radius, initPosition) {
	p_StateMachine = new StateMachine<Character_Ball>(this);
	p_StateMachine->setCurrentState(State_BallTowardsPlayer::instance());
	initMesh();
	resetBall();
}
/*
* Deconstructor
*/
Character_Ball::~Character_Ball() {
	//p_StateMachine->~StateMachine();
}

/*
* Function: initMesh
*
* Paramters: Constants::color
*
* Return Type: void
*
* Description: The parameter takes in the enum value for color and will be set to black
*				by default. This color will be striped on the ball as black and white
*				or another color and black.
*				This function calculates the mesh data for the ball using the member variables.
*
*/
void Character_Ball::initMesh(Constants::color shade) {
	glm::vec3 color1 = Constants::color_pallete[shade];
	glm::vec3 color2 = Constants::color_pallete[Constants::white];
	if (!(shade == Constants::black)) {
		glm::vec3 color2 = Constants::color_pallete[Constants::black];
	}

	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;
	vertices.reserve(626);
	indices.reserve(3600);

	glm::vec3 loc = getLocation();
	Geometry::Vertex topPole = { loc, color1 };
	Geometry::Vertex bottomPole = { { loc.x, loc.y - 2 * getRadius(), loc.z }, color2 };

	vertices.push_back(topPole);

	uint32_t stack = 25, slice = 25;
	float phi = Constants::PI / ((float)stack);
	float theta = (2.0f * Constants::PI) / ((float)slice);

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (uint32_t i = 1; i < stack; i++) {
		float anglePhi = i * phi;

		// Vertices of ring.
		for (uint32_t j = 0; j <= slice; j++) {
			float angleTheta = j * theta;

			Geometry::Vertex point;

			// spherical to cartesian
			point.pos.x = getRadius() * sinf(anglePhi)*cosf(angleTheta);
			point.pos.y = getRadius() * cosf(anglePhi);
			point.pos.z = getRadius() * sinf(anglePhi)*sinf(angleTheta);

			switch (i % 5) {
			case 0:
			case 1:
				point.color = color1;
				break;
			case 2:
				point.color = color2;
				break;
			}

			vertices.push_back(point);
		}
	}
	vertices.push_back(bottomPole);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (uint32_t i = 1; i <= slice; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	uint32_t baseIndex = 1;
	uint32_t ringVertexCount = slice + 1;
	for (uint32_t i = 0; i < stack - 2; i++) {
		for (uint32_t j = 0; j < slice; j++) {
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	uint32_t southPoleIndex = (uint32_t)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (uint32_t i = 0; i < slice; i++) {
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	setGeometry(vertices, indices);
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
bool Character_Ball::runCycle() {
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
void Character_Ball::handleMessages() {
	std::deque<message> mail = getMail();
	while (!(mail.empty())) {
		p_StateMachine->handleMessage(mail.front());
		mail.pop_front();
	}
	readMessages();
}
/*
* Function: reflect
*
* Paramters: none
*
* Return Type: void
*
* Description: changes the member functions to reflect the trajectory
*
*/
void Character_Ball::reflect() {
	setUniformMatrix(glm::translate(glm::mat4(1.0f), getLocation()));
	heading.isReflecting = !heading.isReflecting;
}
/*
* Function: aimAtComputer
*
* Paramters: none
*
* Return Type: void
*
* Description: changes the state of the ball to BallTowardsComputerState
*
*/
void Character_Ball::aimAtComputer() {
	p_StateMachine->setCurrentState(State_BallTowardsComputer::instance());
}
/*
* Function: aimAtUser
*
* Paramters: none
*
* Return Type: void
*
* Description: changes the state of the ball to BallTowardsPlayerState
*
*/
void Character_Ball::aimAtUser() {
	p_StateMachine->setCurrentState(State_BallTowardsPlayer::instance());
}
/*
* Function: setTrajectoryInfo
*
* Paramters: float speed
*			 float degrees
*
* Return Type: void
*
* Description: initializes the trajectory info, used to calculate the uniform matrix
*
*/
void Character_Ball::setTrajectoryInfo(float speed, float degrees) {
	heading.speed = speed;
	heading.degrees = degrees;
	heading.isReflecting = false;

	float axis = glm::radians(heading.degrees - 90.0f);
	heading.rotation_axis = glm::vec3{ cos(axis), 0.0f, sin(axis) };
}
/*
* Function: resetBall
*
* Paramters: none
*
* Return Type: void
*
* Description: initializes and potentially re-initializes the matrix data for
*				the ball character
*
*/
void Character_Ball::resetBall() {
	setLocation(glm::vec3{ 0.0f, getRadius(), 0.0f });
	setUniformMatrix(glm::translate(glm::mat4(1.0f), getLocation()));
	heading.rotation_axis = glm::vec3{ 0.0f, 0.0f, 1.0f };
	heading.speed = 0.0002f;
	heading.degrees = 180.0f;
	heading.isReflecting = false;
	p_StateMachine->setCurrentState(State_BallTowardsPlayer::instance());
}

/*
* 
* The last three functions in this file are getter methods
*
*/
float Character_Ball::getTrajectorySpeed() const {
	return heading.speed;
}
float Character_Ball::getTrajectoryAngle() const {
	return heading.degrees;
}
glm::vec3 Character_Ball::getRoationAxis() const {
	return heading.rotation_axis;
}
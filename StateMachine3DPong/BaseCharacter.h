/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		BaseCharacter.h																*
* DETAILS:	This file defines the base class for the characters to inherit and build	*
*				off. This class essentially handles messages from the DISPATCH			*
*				defined in the Messanger.h, it also defines the getters and setters		*
*				for the various class memebers, and it declares the pure virtual		*
*				functions, which are initMesh, runCycle, and handleMessages.			*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"
#include "Messanger.h"
#include "AIengine.h"

class BaseCharacter {
public:
	BaseCharacter(float rad, glm::vec3 initLocation);
	virtual ~BaseCharacter();

	virtual void initMesh(Constants::color shade) = 0;
	virtual bool runCycle() = 0;
	virtual void handleMessages() = 0;

	void setUniformMatrix(glm::mat4 newMatrix);
	void setLocation(glm::vec3 newLocation);
	void setGeometry(std::vector<Geometry::Vertex> verts, std::vector<uint32_t> inds);

	bool mailReceived();

	void readMessages();

	int getID() const;
	float getRadius() const;
	std::vector<Geometry::Vertex> getVertices() const;
	std::vector<uint32_t> getIndices() const;
	glm::vec3 getLocation() const;
	glm::mat4 getUniformMatrix() const;
	std::deque<message> getMail() const;

private:
	static int next_available_id;

	int id;

	float radius;

	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec3 currLocation;

	glm::mat4 uniformMatrix;

	std::deque<message> mailbox;
};
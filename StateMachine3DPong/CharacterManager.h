/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		CharacterManager.h															*
* DETAILS:	This file defines the class that will work as the overall manager of all	*
*				the states, the physics, the ai, the messanger, and all of the			*
*				characters. This, in the window class, will interface with the render	*
*				engine to form the game.												*
*																						*
*****************************************************************************************/


#pragma once
#include "stdafx.h"
#include "BaseCharacter.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"

class CharacterManager {
public:
	CharacterManager();
	~CharacterManager();

	void AddCharacter(BaseCharacter * newCharacter);
	void DeleteCharacter(BaseCharacter * character);

	void updateOffsets();

	bool runFSMs();

	void userInput(bool up, bool down, int user_id);
	
	std::vector<glm::mat4> getUniformMatrices() const;
	Geometry::GeometryManager* getOffsets() const;

private:
	typedef std::map<int, BaseCharacter*> characterMap;

	characterMap MasterList;

	Geometry::GeometryManager * p_Offsets;

	std::vector<glm::mat4> uniformMatrices;

	void initScenicCharacters();
	void updateMatrices();
};
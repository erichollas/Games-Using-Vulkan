/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Window.h																	*
* DETAILS:	This file defines the Window class to present the rendered images to the	*
*				display. It also manages the clock object for frame stats and the		*
*				camera object for the user point of view. Also, the window will act		*
*				as the user interface for the user taking in input and dispersing it	*
*				the input to the Messanger. Since so many things are being called		*
*				and handled in this file. The main loop for the game will run in this	*
*				object and the render engine will be managed in this file. Thus the		*
*				window object will called and run the main method.						*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/Clock.h"
#include "../../VulkanRenderEngine/RenderEngine/RenderEngine.h"
#include "CharacterManager.h"
#include "Messanger.h"

class Window {
public:
	Window() throw(Exception);
	~Window();

	void runGame() throw(Exception);

	static void keyCallback(GLFWwindow *pW, int key, int scancode, int action, int mods);

private:
	GLFWwindow * pWindow;
	Camera::pointOfView * pEyeOfTheBeholder;

	RenderEngine renderer;
	CharacterManager CharacterList;

	static int user_id;
	static Clock timer;
	static Messanger dispatch;

	void initCamera();
	void initCharacters();
	void displayFrameStats();
};

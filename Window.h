/****************************************************************************************
* TITLE:	Multi-threaded Minesweeper													*
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
#include "TileManager.h"

// gameState will be used to track the game's state
enum gameState {
	lost,
	reset,
	ready,
	playing
};

class Window {
public:
	Window();
	~Window();

	void runGame();

	static void keyCallback(GLFWwindow *pW, int key, int scancode, int action, int mods);
	static void mouse_position_callback(GLFWwindow *pW, double x_pos, double y_pos);
	static void mouse_click_callback(GLFWwindow *pW, int button, int action, int mods);

private:
	GLFWwindow * pWindow;
	Camera::pointOfView * pEyeOfTheBeholder;

	TileManager GameBoard;

	RenderEngine renderer;

	static clock_t game_time;
	static gameState state;
	static double xPosition;
	static double yPosition;
	static bool isMouseDown;
	static bool isLeftClick;
	static int user_id;
	static Clock timer;

	void initCamera();
	void displayFrameStats();
};
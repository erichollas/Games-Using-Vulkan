#pragma once
#include "stdafx.h"
#include "Clock.h"
#include "RenderEngine.h"
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
	Window() throw(Exception);
	~Window();

	void runGame() throw(Exception);

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
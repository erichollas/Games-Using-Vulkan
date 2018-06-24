/****************************************************************************************
* TITLE:	Multi-threaded Minesweeper													*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Window.cpp																	*
*																						*
* Correponds to Window.h (see for overall description)									*
*																						*
*****************************************************************************************/

#include "stdafx.h"
#include "Window.h"
#include "TileGeometry.h"
#include "MouseClickReader.h"


//declaration of static member variables 
// (in this instance these variables need to be static as they 
//  will be called in the callback functions)
Clock Window::timer = Clock();
int Window::user_id = -1;
double Window::xPosition = 0.0;
double Window::yPosition = 0.0;
bool Window::isMouseDown = false;
bool Window::isLeftClick = true;
gameState Window::state = ready;
clock_t Window::game_time = 0.0f;

/*
* Constructor
*
* initializes the GLFWwindow variable
*
*/
Window::Window() {
	pEyeOfTheBeholder = new Camera::pointOfView();

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	pWindow = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);

	glfwSetKeyCallback(pWindow, keyCallback);
	glfwSetCursorPosCallback(pWindow, mouse_position_callback);
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetMouseButtonCallback(pWindow, mouse_click_callback);
}
/*
* Deconstructor
*/
Window::~Window() {
	glfwDestroyWindow(pWindow);
	glfwTerminate();
}

/*
* Function: runGame
*
* Paramters: none
*
* Return Type: void
*
* Description: runs the main game loop
*
*/
void Window::runGame() {
	GameBoard = TileManager();
	GameBoard.setupMines();

	Geometry::GeometryManager offsets = GameBoard.getGeometryInfo();
	renderer.initVulkan(pWindow, offsets);

	initCamera();
	Camera::UniformBufferObject ubo;
	ubo.proj = pEyeOfTheBeholder->getPerspectiveMatrix();
	ubo.view = pEyeOfTheBeholder->getLookAtMatrix();

	int frameCount = 0;	//used to ensure that the timer per game clock() object is called 
						//twice a second, which will drastically increase performance
	int nonMineClicks = 0;	//will be used to check if a game has been won

	while (!glfwWindowShouldClose(pWindow)) {
		bool updateVectors = false;	//will only be true if vector information has changed to ensure that
									//the vector information will only be sent to the gpu once per loop-cycle

		timer.startTimer();
		glfwPollEvents();

		//this if statement checks if the clock needs to be updated will a game is being played
		if (state == playing && frameCount == 0) {
			clock_t temp_time = clock() - game_time;
			if (GameBoard.updateTime((((float)temp_time) / CLOCKS_PER_SEC))) {
				updateVectors = true;
			}
		}

		//the first if checks to see if the game needs initializing for a new game or otherwise
		//	the else if checks to see if a mouse click has happened and needs to be processed
		if (state == reset) {
			nonMineClicks = 0;
			GameBoard.setupMines();
			updateVectors = true;
			state = ready;
		}
		else if (isMouseDown) {
			//the first if is for left clicks, the else is for right clicks
			if (isLeftClick) {
				bool dataChanged = false;
				//if checks if a mine was hit
				if (GameBoard.processLeftClick(PICKER->getVulkanCoordinates().x, PICKER->getVulkanCoordinates().y, dataChanged)) {
					state = lost;
					updateVectors = true;
				}
				else if (dataChanged) {
					updateVectors = true;
					nonMineClicks++;
				}
			}
			else {
				GameBoard.processRightClick(PICKER->getVulkanCoordinates().x, PICKER->getVulkanCoordinates().y);
				updateVectors = true;
			}
			isMouseDown = false; // reset to false to ensure one click per cycle is processed
		}

		//updates the geometry info to the gpu
		if (updateVectors) {
			offsets = GameBoard.getGeometryInfo();
			renderer.updateGeometryBuffers(offsets);
		}

		//passes the uniform matrices and renders the frame
		renderer.updateUniformBuffer(ubo, GameBoard.getUniformMatrices());
		renderer.drawFrame();

		//checks if a game has been won
		if (nonMineClicks == (GAMEBOARD_ROWS * GAMEBOARD_ROWS) - NUM_OF_MINES) {
			state = lost;
		}

		frameCount++;
		if (frameCount == 500) {
			frameCount = 0;
		}

		timer.calcFrameStats();
		displayFrameStats();
	}

	renderer.cleanup();
}


/*
* Function: keyCallback
*
* Paramters: standard parameters for the call back function
*
*
* Description: the standard key callback function for the GLFWwindow
*
*/
void Window::keyCallback(GLFWwindow *pW, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(pW, GLFW_TRUE);
	}
	if (key == GLFW_KEY_SPACE) {
		state = reset;
	}
}
/*
* Function: mouse_position_callback
*
* Paramters: standard parameters for the call back function
*
*
* Description: tracks the position of the cursor
*
*/
void Window::mouse_position_callback(GLFWwindow *pW, double x_pos, double y_pos) {
	xPosition = x_pos;
	yPosition = y_pos;
}
/*
* Function: mouse_click_callback
*
* Paramters: standard parameters for the call back function
*
*
* Description: records cursor position when a click occurs in PICKER object, which is of 
*				type static MouseClickReader*
*
*/
void Window::mouse_click_callback(GLFWwindow *pW, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && state != lost) {
		if (state != playing) {
			state = playing;
			game_time = clock();
		}
		isMouseDown = true;
		isLeftClick = true;
		PICKER->setClick((float)xPosition, (float)yPosition);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && state != lost) {
		if (state != playing) {
			state = playing;
			game_time = clock();
		}
		isMouseDown = true;
		isLeftClick = false;
		PICKER->setClick((float)xPosition, (float)yPosition);
	}
}


/*
* Function: initCamera
*
* Paramters: none
*
* Return Type: void
*
* Description: initializes the values for the camera object used for
*				the non dynamic portion of the uniform buffer
*
*/
void Window::initCamera() {
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);

	pEyeOfTheBeholder->eye = glm::vec3(0.0f, 0.0f, 2.5f);
	pEyeOfTheBeholder->at = glm::vec3(0.0f, 0.0f, 0.0f);
	pEyeOfTheBeholder->up = glm::vec3(0.0f, -1.0f, 0.0f);
	pEyeOfTheBeholder->fovy = glm::radians(45.0f);
	pEyeOfTheBeholder->ratio = width / ((float)height);
	pEyeOfTheBeholder->nearPerspective = 0.1f;
	pEyeOfTheBeholder->farPerspective = 10.0f;
}
/*
* Function: displayFrameStats
*
* Paramters: none
*
* Return Type: void
*
* Description: displays the calculated frame stats from the clock object
*				note that for accurate results calcFrameStats must be called before this
*				method that is a member function for the clock object
*
*/
void Window::displayFrameStats() {
	std::string updatedTitle = WINDOW_TITLE;
	updatedTitle += "   ";
	updatedTitle += "avg. fps: ";
	updatedTitle += std::to_string(timer.getFPS());
	updatedTitle += "  avg. mspf: ";
	updatedTitle += std::to_string(timer.getMSPF());
	glfwSetWindowTitle(pWindow, updatedTitle.c_str());
}
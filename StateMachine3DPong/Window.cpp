/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Window.cpp																	*
*																						*
* Correponds to Window.h (see for overall description)									*
*																						*
*****************************************************************************************/

#include "stdafx.h"
#include "Window.h"
#include "Character_Paddle.h"
#include "Character_Ball.h"
#include "Character_Score.h"

//declaration of static member variables 
Messanger Window::dispatch = Messanger();
Clock Window::timer = Clock();
int Window::user_id = -1;

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
	initCharacters();
	renderer.initVulkan(pWindow, *CharacterList.getOffsets());
	initCamera();
	Camera::UniformBufferObject ubo;
	ubo.proj = pEyeOfTheBeholder->getPerspectiveMatrix();
	ubo.view = pEyeOfTheBeholder->getLookAtMatrix();

	while (!glfwWindowShouldClose(pWindow)) {
		glfwPollEvents();

		timer.startTimer();

		bool updateOffsets = CharacterList.runFSMs();

		if (updateOffsets) {
			CharacterList.updateOffsets();
			renderer.updateGeometryBuffers(*CharacterList.getOffsets());
		}

		renderer.updateUniformBuffer(ubo, CharacterList.getUniformMatrices());
		renderer.drawFrame();

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
	else if (key == GLFW_KEY_UP) {
		DISPATCH->collect(move_up, user_id, -1);
	}
	else if (key == GLFW_KEY_DOWN) {
		DISPATCH->collect(move_down, user_id, -1);
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

	pEyeOfTheBeholder->eye = glm::vec3(0.0f, 1.25f, 2.5f);
	pEyeOfTheBeholder->at = glm::vec3(0.0f, 0.0f, 0.0f);
	pEyeOfTheBeholder->up = glm::vec3(0.0f, -1.0f, 0.0f);
	pEyeOfTheBeholder->fovy = glm::radians(45.0f);
	pEyeOfTheBeholder->ratio = width / ((float)height);
	pEyeOfTheBeholder->nearPerspective = 0.1f;
	pEyeOfTheBeholder->farPerspective = 10.0f;
}
/*
* Function: initCharacters
*
* Paramters: none
*
* Return Type: void
*
* Description: initializes the characters with their initial data for the game
*				since this is a small game, it is plain to see that the characters
*				id's obvious in this method from the order they are instanstiated
*
*/
void Window::initCharacters() {
	Character_Paddle * pUser, *pComputer;
	Character_Ball * pBall;
	Character_Score * pHomeScore, * pAwayScore;

	pUser = new Character_Paddle(0.125f, glm::vec3{ -0.9f, 0.0f, 0.0f });		//user_id = 0
	pComputer = new Character_Paddle(0.125f, glm::vec3{ 1.0f, 0.0f, 0.0f });	//computer_id = 1
	pBall = new Character_Ball(0.035f, glm::vec3{ 0.0f, 0.035f, 0.0f });		//ball_id = 2
	pHomeScore = new Character_Score(glm::vec3{ 0.0f, 0.0f, 0.0f });			//homeScore_id = 3
	pAwayScore = new Character_Score(glm::vec3{ 1.5f, 0.0f, 0.0f });			//awayScore_id = 4

	pUser->initMesh(Constants::blue);
	pComputer->initMesh(Constants::red);
	pHomeScore->initMesh();
	pAwayScore->initMesh();

	user_id = pUser->getID();

	CharacterList.AddCharacter(pUser);
	CharacterList.AddCharacter(pComputer);
	CharacterList.AddCharacter(pBall);
	CharacterList.AddCharacter(pHomeScore);
	CharacterList.AddCharacter(pAwayScore);
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
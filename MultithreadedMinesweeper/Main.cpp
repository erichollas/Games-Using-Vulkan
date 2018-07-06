/*
* TITLE:	Multi-threaded Minesweeper
* BY:		Eric Hollas
*	
* FILE:		main.cpp
* DETAILS:	This file creates and calls the object to run the game						*
*/
#include "stdafx.h"
#include "Window.h"
#include "Exception.h"

int main() {
	try {
		Window app;
		app.runGame();
	}
	catch (Exception &excpt) {
		std::cout << excpt.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
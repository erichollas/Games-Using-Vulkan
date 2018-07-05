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
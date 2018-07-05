/*
* TITLE:	Vulkan Render Engine
* BY:		Eric Hollas
*
* FILE:		Exception.h
* DETAILS:	This file defines the Exception class that will be used to detail
*				thrown exceptions throughout the Render Engine project and likely
*				the Window Class when the renderring engine is implemented.
*/
#pragma once
#include <iostream>
#include <string>

class Exception {
public:
	Exception(std::string msg, std::string fl, std::string fn) {
		message = msg;
		file = fl;
		function = fn;
	}
	~Exception() {
	}

	std::string what() {
		std::string wh = "Exception thrown:  -file- ";
		wh += file;
		wh += ",  -function- ";
		wh += function;
		wh += ",  -feedback- ";
		wh += message;

		return wh;
	}
private:
	std::string message;
	std::string file;
	std::string function;

};
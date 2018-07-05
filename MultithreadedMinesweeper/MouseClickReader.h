#ifndef _MOUSECLICKREADER_H_
#define _MOUSECLICKREADER_H_

#include "stdafx.h"

class MouseClickReader {
public:
	static MouseClickReader* instance();

	/*
	* Function: setClick
	*
	* Paramters: float x_position
	*			 float y_position
	*
	* Return Type: void
	*
	* Description: converts the parameters from glfw screen coordinates to
	*				the coordinates used by vulkan (given the camera, which
	*				is stagnent, view and projection). It then stores the
	*				values in vulkan_coordinates member variable to be returned later
	*
	*/
	void setClick(float x_position, float y_position) {
		vulkan_coordinates.x = 1.03f - x_position * (2.06f / DEFAULT_WINDOW_WIDTH);
		vulkan_coordinates.y = 1.03f - y_position * (2.06f / DEFAULT_WINDOW_HEIGHT);
	}
	// a simple getter method
	glm::vec2 getVulkanCoordinates() const {
		return vulkan_coordinates;
	}
private:
	glm::vec2 vulkan_coordinates;
};

//returns a static instance of the object, will be defined as the constant PICKER
MouseClickReader* MouseClickReader::instance() {
	static MouseClickReader inst;
	return &inst;
}

#endif /*_MOUSECLICKREADER_H_*/

//defines the instance of the object
#define PICKER MouseClickReader::instance()
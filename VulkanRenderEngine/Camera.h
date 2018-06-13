/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Camera.h																	*
* DETAILS:	This file describes the camera object and is made to store the constant		*
*				uniform buffer data to the shaders. Also, it is made to easily change	*
*				the values in the Window class for which it will be apart.				*
*																						*
*****************************************************************************************/

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


namespace Camera {
	struct UniformBufferObject {
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct pointOfView {
		glm::vec3 eye, at, up;
		float fovy, ratio, nearPerspective, farPerspective;

		pointOfView() {}

		glm::mat4 getLookAtMatrix() const {
			return glm::lookAt(eye, at, up);
		}
		glm::mat4 getPerspectiveMatrix() const {
			return glm::perspective(fovy, ratio, nearPerspective, farPerspective);
		}
	};
}
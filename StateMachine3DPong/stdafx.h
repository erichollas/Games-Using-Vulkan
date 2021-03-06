/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		stdafx.h																	*
* DETAILS:	This file is required to be included throughout the project files as		*
*				the pre-compiled header files. Thus I put all external header files		*
*				used throughout the project and several constants in this file.			*
*																						*
*****************************************************************************************/

#pragma once

#define NOMINMAX
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <array>
#include <set>
#include <map>
#include <deque>

#define DEFAULT_WINDOW_WIDTH  2000
#define DEFAULT_WINDOW_HEIGHT 1600

#define WINDOW_TITLE "3D-PONG"
#define APP_NAME "Title"
#define ENGINE_NAME "Title"

#define VERTEX_SHADER_PATH   "Shaders/vert.spv"
#define FRAGMENT_SHADER_PATH "Shaders/frag.spv"

#ifdef NDEBUG
const bool isDebugging = false;
#else
const bool isDebugging = true;
#endif

namespace Constants {
	const float PI = 3.141592653f;
	const float ACCURACY_EPSILON = 0.00001f;

	enum color {
		white,
		red,
		brown,
		darkBrown,
		green,
		greenGrass,
		blue,
		magenta,
		yellow,
		teal,
		black,
		gray
	};
	const std::vector<glm::vec3> color_pallete = {
		{ 1.0f, 1.0f, 1.0f }, //white
		{ 1.0f, 0.0f, 0.0f }, //red
		{ 0.5f, 0.3f, 0.0f }, //brown
		{ 0.3f, 0.2f, 0.0f }, //darkBrown
		{ 0.0f, 1.0f, 0.0f }, //green
		{ 0.0f, 0.4f, 0.1f }, //greenGrass
		{ 0.0f, 0.0f, 1.0f }, //blue
		{ 1.0f, 0.0f, 1.0f }, //magenta
		{ 1.0f, 1.0f, 0.0f }, //yellow
		{ 0.0f, 1.0f, 1.0f }, //teal
		{ 0.0f, 0.0f, 0.0f }, //black
		{ 0.5f, 0.5f, 0.5f }  //gray
	};

	static glm::vec3 GetClearColor() {
		return color_pallete[black];
	}
}

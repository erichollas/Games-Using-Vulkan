/*
* TITLE:	Multi-threaded Minesweeper
* BY:		Eric Hollas
*
* FILE:		stdafx.h
* DETAILS:	This file is required to be included throughout the project files as
*				the pre-compiled header files. 
*/
#pragma once
#define NOMINMAX
#define NOMCX
#define NOSERVICE
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <time.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <array>
#include <set>
#include <map>
#include <deque>
#include <thread>

#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 800

#define WINDOW_TITLE "Multi-threaded Minesweeper"
#define APP_NAME "Title"
#define ENGINE_NAME "Title"

#define VERTEX_SHADER_PATH   "Shaders/vert.spv"
#define FRAGMENT_SHADER_PATH "Shaders/frag.spv"

#ifdef NDEBUG
const bool isDebugging = false;
#else
const bool isDebugging = true;
#endif
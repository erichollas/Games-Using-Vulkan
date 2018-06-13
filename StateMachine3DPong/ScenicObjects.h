/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		ScenicObjects.h																*
* DETAILS:	This file defines the scenary objects and their mesh data.					*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"

struct homeGoalLine {
	const std::vector<Geometry::Vertex> vertices = {
		{ { -1.0f, 0.0f,  0.8f }, Constants::color_pallete[Constants::red] },     //index 0
		{ { -0.9f, 0.0f,  0.8f }, Constants::color_pallete[Constants::gray] },    //index 1
		{ { -0.9f, 0.0f, -0.8f }, Constants::color_pallete[Constants::gray] },    //index 2
		{ { -1.0f, 0.0f, -0.8f }, Constants::color_pallete[Constants::red] }      //index 3
	};

	const std::vector<uint32_t> indices = {
		0, 3, 2,
		2, 1, 0
	};
};

struct awayGoalLine {
	const std::vector<Geometry::Vertex> vertices = {
		{ { 0.9f, 0.0f,  0.8f }, Constants::color_pallete[Constants::gray] },     //index 0
		{ { 1.0f, 0.0f,  0.8f }, Constants::color_pallete[Constants::blue] },     //index 1
		{ { 1.0f, 0.0f, -0.8f }, Constants::color_pallete[Constants::blue] },     //index 2
		{ { 0.9f, 0.0f, -0.8f }, Constants::color_pallete[Constants::gray] }      //index 3
	};

	const std::vector<uint32_t> indices = {
		0, 3, 2,
		2, 1, 0
	};
};

struct midFieldLine {
	const std::vector<Geometry::Vertex> vertices = {
		{ { -0.05f, 0.0f,  0.8f }, Constants::color_pallete[Constants::gray] },    //index 0
		{ { 0.05f, 0.0f,  0.8f }, Constants::color_pallete[Constants::gray] },     //index 1
		{ { 0.05f, 0.0f, -0.8f }, Constants::color_pallete[Constants::gray] },     //index 2
		{ { -0.05f, 0.0f, -0.8f }, Constants::color_pallete[Constants::gray] }     //index 3
	};

	const std::vector<uint32_t> indices = {
		0, 3, 2,
		2, 1, 0
	};
};

struct homeField {
	const std::vector<Geometry::Vertex> vertices = {
		{ { -0.9f,  0.0f,  0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 0
		{ { -0.05f, 0.0f,  0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 1
		{ { -0.05f, 0.0f, -0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 2
		{ { -0.9f,  0.0f, -0.8f }, Constants::color_pallete[Constants::greenGrass] }      //index 3
	};

	const std::vector<uint32_t> indices = {
		0, 3, 2,
		2, 1, 0
	};
};

struct awayField {
	const std::vector<Geometry::Vertex> vertices = {
		{ { 0.05f, 0.0f,  0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 0
		{ { 0.9f,  0.0f,  0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 1
		{ { 0.9f,  0.0f, -0.8f }, Constants::color_pallete[Constants::greenGrass] },     //index 2
		{ { 0.05f, 0.0f, -0.8f }, Constants::color_pallete[Constants::greenGrass] }      //index 3
	};

	const std::vector<uint32_t> indices = {
		0, 3, 2,
		2, 1, 0
	};
};

struct nearBarrier {
	const std::vector<Geometry::Vertex> vertices = {
		//front
		{ { -1.0f, 0.0f, 1.0f }, Constants::color_pallete[Constants::brown] },		//index 0
		{ { -1.0f, 0.1f, 1.0f }, Constants::color_pallete[Constants::darkBrown] },  //index 1
		{ { 1.0f, 0.1f, 1.0f }, Constants::color_pallete[Constants::darkBrown] },   //index 2
		{ { 1.0f, 0.0f, 1.0f }, Constants::color_pallete[Constants::brown] },		//index 3

		//top
		{ { -1.0f, 0.1f, 0.8f }, Constants::color_pallete[Constants::brown] },		//index 5
		{ { 1.0f, 0.1f, 0.8f }, Constants::color_pallete[Constants::brown] }		//index 6
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 3, 0,

		1, 4, 5,
		5, 2, 1
	};
};

struct farBarrier {
	const std::vector<Geometry::Vertex> vertices = {
		//front
		{ { -1.0f, 0.0f, -0.8f }, Constants::color_pallete[Constants::brown] },     //index 0
		{ { -1.0f, 0.1f, -0.8f }, Constants::color_pallete[Constants::darkBrown] }, //index 1
		{ { 1.0f, 0.1f, -0.8f }, Constants::color_pallete[Constants::darkBrown] },	//index 2
		{ { 1.0f, 0.0f, -0.8f }, Constants::color_pallete[Constants::brown] },		//index 3

		//top
		{ { -1.0f, 0.1f, -1.0f }, Constants::color_pallete[Constants::brown] },     //index 4
		{ { 1.0f, 0.1f, -1.0f }, Constants::color_pallete[Constants::brown] }		//index 5
	};

	const std::vector<uint32_t> indices = {
		0, 1, 2,
		2, 3, 0,

		1, 4, 5,
		5, 2, 1
	};
};
/****************************************************************************************
* TITLE:	2D-Pong in 3D																*
* BY:		Eric Hollas																	*
*																						*
* FILE:		Numbers.h																	*
* DETAILS:	This file defines the mesh data for the numbers to be used in the			*
*				Scoreboard object.														*
*																						*
*****************************************************************************************/

#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"

/*
* Struct: the Number struct is the base class for the other numbers
*			to be used as characters in the scoreboard object
*	Basically these characters will only contain vertex and index data
*
*/
struct Number {
	glm::vec3 color;
	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;

	void setColor(glm::vec3 cl) {
		color = cl;
	}
	virtual void initInfo() = 0;
};

/*
* The following structs are used to store the render data for 
*	the scoreboard object. All of them inherit from the Number
*	object described above. Note that for the data to be set
*	initInfo() must first be called, and before initInfo() is
*	called you need to have first set the color with the setColor function
*
*/
struct zero : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//right
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 6
			{ { -0.8f, 0.5f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//left
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 12
			{ { -0.7f, 0.2f, -0.8f }, color },	//index 13
			{ { -0.7f, 0.5f, -0.8f }, color },	//index 14
			{ { -0.6f, 0.5f, -0.8f }, color }   //index 15
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12
		};
	}
};

struct one : public Number {
	void initInfo() override{
		vertices = {
			//top
			{ { -0.8f,   0.5f, -0.8f }, color },  //index 0
			{ { -0.8f,   0.6f, -0.8f }, color },	//index 1
			{ { -0.625f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.625f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.7f, 0.2f, -0.8f }, color },  //index 4
			{ { -0.8f, 0.2f, -0.8f }, color },	//index 5
			{ { -0.8f, 0.5f, -0.8f }, color },	//index 6
			{ { -0.7f, 0.5f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8
		};
	}
};

struct two : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//right
			{ { -0.8f, 0.425f, -0.8f }, color },  //index 12
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 13
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 14
			{ { -0.8f, 0.5f, -0.8f }, color },  //index 15

			//left
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 16
			{ { -0.7f, 0.2f, -0.8f }, color },	//index 17
			{ { -0.7f, 0.375f, -0.8f }, color },	//index 18
			{ { -0.6f, 0.375f, -0.8f }, color }   //index 19
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16
		};
	}
};

struct three : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//right
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 6
			{ { -0.8f, 0.5f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 12
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 13
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 14
			{ { -0.6f, 0.425f, -0.8f }, color }  //index 15
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12
		};
	}
};

struct four : public Number {
	void initInfo() override {
		vertices = {
			//right
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 1
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.8f, 0.6f, -0.8f }, color },  //index 3

			//left
			{ { -0.6f, 0.4f, -0.8f }, color },  //index 4
			{ { -0.7f, 0.4f, -0.8f }, color },	//index 5
			{ { -0.7f, 0.6f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.6f, -0.8f }, color },  //index 7

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.425f, -0.8f }, color }  //index 11
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8
		};
	}
};

struct five : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//left
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 12
			{ { -0.7f, 0.425f, -0.8f }, color },	//index 13
			{ { -0.7f, 0.5f, -0.8f }, color },	//index 14
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 15

			//right
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 16
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 17
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 18
			{ { -0.8f, 0.375f, -0.8f }, color }   //index 19
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16
		};
	}
};

struct six : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//right
			{ { -0.8f, 0.3f,   -0.8f }, color },  //index 12
			{ { -0.9f, 0.3f,   -0.8f }, color },	//index 13
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 14
			{ { -0.8f, 0.375f, -0.8f }, color },  //index 15

			//left
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 16
			{ { -0.7f, 0.2f, -0.8f }, color },	//index 17
			{ { -0.7f, 0.5f, -0.8f }, color },	//index 18
			{ { -0.6f, 0.5f, -0.8f }, color }   //index 19
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16
		};
	}
};

struct seven : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//right
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 6
			{ { -0.8f, 0.5f, -0.8f }, color }  //index 7
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};
	}
};

struct eight : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//right
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 12
			{ { -0.7f, 0.2f, -0.8f }, color },	//index 13
			{ { -0.7f, 0.5f, -0.8f }, color },	//index 14
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 15

			//left
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 16
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 17
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 18
			{ { -0.8f, 0.5f, -0.8f }, color }   //index 19
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16
		};
	}
};

struct nine : public Number {
	void initInfo() override {
		vertices = {
			//top
			{ { -0.9f, 0.5f, -0.8f }, color },  //index 0
			{ { -0.9f, 0.6f, -0.8f }, color },	//index 1
			{ { -0.6f, 0.6f, -0.8f }, color },	//index 2
			{ { -0.6f, 0.5f, -0.8f }, color },  //index 3

			//middle
			{ { -0.6f, 0.375f, -0.8f }, color },  //index 4
			{ { -0.9f, 0.375f, -0.8f }, color },	//index 5
			{ { -0.9f, 0.425f, -0.8f }, color },	//index 6
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 7

			//bottom
			{ { -0.6f, 0.2f, -0.8f }, color },  //index 8
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 9
			{ { -0.9f, 0.3f, -0.8f }, color },	//index 10
			{ { -0.6f, 0.3f, -0.8f }, color },  //index 11

			//right
			{ { -0.6f, 0.425f, -0.8f }, color },  //index 12
			{ { -0.7f, 0.425f, -0.8f }, color },	//index 13
			{ { -0.7f, 0.5f,   -0.8f }, color },	//index 14
			{ { -0.6f, 0.5f,   -0.8f }, color },  //index 15

			//left
			{ { -0.8f, 0.2f, -0.8f }, color },  //index 16
			{ { -0.9f, 0.2f, -0.8f }, color },	//index 17
			{ { -0.9f, 0.5f, -0.8f }, color },	//index 18
			{ { -0.8f, 0.5f, -0.8f }, color }   //index 19
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16
		};
	}
};
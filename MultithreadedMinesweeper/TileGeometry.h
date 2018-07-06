/*
* TITLE:	Multi-threaded Minesweeper
* BY:		Eric Hollas
*
* FILE:		TileGeometry.h	
* DETAILS:	This file defines the geometric data used for each tile. Each tile will
*				inherit from the BaseTileGeometry virtual class. Each object will only
*				define the pure virtual function initMesh() to initialize the geometric
*				data used for renderring each tile.	
*			Each object is named consistently, there will be a tile for blank tile, an
*				unclicked tile, a tile for a mine clicked on and a mine reveal, a tile
*				that is flagged as a potential mine, and a tile for each number 0-9.
*			The mine image is that of the "nuclear symbol" its geometry info is several
*				tessellated circles.
*/

#pragma once
#include "stdafx.h"
#include "GeometryManager.h"

struct BaseTileGeometry {
	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;

	virtual void initMesh() = 0;
};

struct blank_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) }
		};
		indices = {
			0, 1, 2,
			2, 3, 0
		};
	}
};
struct unshown_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ { -0.1f, 0.1f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 0
			{ { -0.9f, 0.1f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 1
			{ { -0.9f, 0.9f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 2
			{ { -0.1f, 0.9f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 3

			{ { -0.9f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 4
			{ { -0.1f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 5

			{ { -1.0f, 0.1f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 6
			{ { -1.0f, 0.9f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 7

			{ {  0.0f, 0.1f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 8
			{ {  0.0f, 0.9f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 9

			{ { -0.1f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 10
			{ { -0.9f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 11

			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 12
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 13
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 14
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) }	//index 15

		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			1, 6, 7,
			7, 2, 1,

			2, 4, 5,
			5, 3, 2,

			8, 0, 3,
			3, 9, 8,

			10, 11, 1,
			1, 0, 10,

			12, 10, 0,
			0, 8, 12,

			13, 6, 1,
			1, 11, 13,

			14, 4, 2,
			2, 7, 14,

			15, 9, 3,
			3, 5, 15
		};
	}
};
struct flagged_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ { -0.1f, 0.1f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 0
			{ { -0.9f, 0.1f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 1
			{ { -0.9f, 0.9f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 2
			{ { -0.1f, 0.9f, 0.0f }, Geometry::getColor(Geometry::gray) },		//index 3

			{ { -0.9f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 4
			{ { -0.1f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 5

			{ { -1.0f, 0.1f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 6
			{ { -1.0f, 0.9f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 7

			{ {  0.0f, 0.1f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 8
			{ {  0.0f, 0.9f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 9

			{ { -0.1f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 10
			{ { -0.9f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 11

			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 12
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 13
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 14
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::darkGray) },	//index 15

			{ { -0.2f, 0.2f, 0.1f }, Geometry::getColor(Geometry::darkBrown) },	//index 16
			{ { -0.8f, 0.2f, 0.1f }, Geometry::getColor(Geometry::darkBrown) },	//index 17
			{ { -0.8f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkBrown) },	//index 18
			{ { -0.2f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkBrown) },	//index 19

			{ { -0.25f, 0.3f,  0.1f }, Geometry::getColor(Geometry::black) },	//index 20
			{ { -0.75f, 0.3f,  0.1f }, Geometry::getColor(Geometry::black) },	//index 21
			{ { -0.75f, 0.35f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 22
			{ { -0.25f, 0.35f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 23

			{ { -0.65f, 0.35f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 24
			{ { -0.7f,  0.35f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 25
			{ { -0.7f,  0.75f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 26
			{ { -0.65f, 0.75f, 0.1f }, Geometry::getColor(Geometry::black) },	//index 27

			{ { -0.65f, 0.75f, 0.1f }, Geometry::getColor(Geometry::red) },		//index 28
			{ { -0.35f, 0.65f, 0.1f }, Geometry::getColor(Geometry::red) },		//index 29
			{ { -0.65f, 0.55f, 0.1f }, Geometry::getColor(Geometry::red) }		//index 30
		};
		indices = {
			0, 1, 2,
			2, 3, 0,

			1, 6, 7,
			7, 2, 1,

			2, 4, 5,
			5, 3, 2,

			8, 0, 3,
			3, 9, 8,

			10, 11, 1,
			1, 0, 10,

			12, 10, 0,
			0, 8, 12,

			13, 6, 1,
			1, 11, 13,

			14, 4, 2,
			2, 7, 14,

			15, 9, 3,
			3, 5, 15,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20,

			24, 25, 26,
			26, 27, 24,

			28, 29, 30
		};
	}
};
struct mine_hit_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::red) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::red) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::red) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::red) }
		};
		indices = {
			0, 1, 2,
			2, 3, 0
		};

		const float PI = 3.141592653f;
		float radius = 0.4f;						//size of the mine
		int slice = 16;								//how many triangles will be used
		uint32_t fixed_index = vertices.size();		//starting index for the circle objects
		float angle = PI / (float)slice;			//the angle to be incremented between triangles
		Geometry::color cl = Geometry::yellow;		//the color of triangles

		Geometry::Vertex center_circle = { { -0.5f, 0.5f, 0.02f }, Geometry::getColor(cl) };
		Geometry::Vertex first_point = { { -0.5f - radius, 0.5f, 0.02f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//yellow circle
		uint32_t max_loop_count = ((2 * slice) + (fixed_index - 2));
		for (uint32_t index = (fixed_index + 2); index < max_loop_count; index++) {
			float x_component = -0.5f - radius * cos((index - (fixed_index - 1)) * angle);
			float y_component = 0.5f + radius * sin((index - (fixed_index - 1)) * angle);
			Geometry::Vertex point = { { x_component, y_component, 0.02f }, Geometry::getColor(cl) };
			vertices.push_back(point);
			indices.push_back(fixed_index);
			indices.push_back(index - 1);
			indices.push_back(index);
		}

		indices.push_back(fixed_index);
		indices.push_back(vertices.size() - 1);
		indices.push_back(fixed_index + 1);

		//inner most black circle
		fixed_index = vertices.size();
		cl = Geometry::black;
		radius = 0.1f;

		center_circle = { { -0.5f, 0.5f, 0.04f }, Geometry::getColor(cl) };
		first_point = { { -0.5f - radius, 0.5f, 0.04f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//inner most black circle
		max_loop_count = ((2 * slice) + (fixed_index - 2));
		for (uint32_t index = (fixed_index + 2); index < max_loop_count; index++) {
			float x_component = -0.5f - radius * cos((index - (fixed_index - 1)) * angle);
			float y_component = 0.5f + radius * sin((index - (fixed_index - 1)) * angle);
			Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
			vertices.push_back(point);
			indices.push_back(fixed_index);
			indices.push_back(index - 1);
			indices.push_back(index);
		}

		indices.push_back(fixed_index);
		indices.push_back(vertices.size() - 1);
		indices.push_back(fixed_index + 1);

		//middle ring pieces
		radius = 0.2f;
		float outter_radius = 0.35f;
		fixed_index = vertices.size();

		angle /= 2.0f;

		center_circle = { { -0.5f - radius, 0.5f, 0.04f }, Geometry::getColor(cl) };
		first_point = { { -0.5f - outter_radius, 0.5f, 0.04f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//first middle ring piece
		for (uint32_t index = 1; index < 56; index += 2) {
			if (index < 12 || (index > 22 && index < 34 || (index > 42))) {
				float x_component = -0.5f - radius * cos(index * angle);
				float y_component = 0.5f + radius * sin(index * angle);
				Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				x_component = -0.5f - outter_radius * cos(index * angle);
				y_component = 0.5f + outter_radius * sin(index * angle);
				point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				if (index < 12) {
					indices.push_back(fixed_index + index - 1);
					indices.push_back(fixed_index + index);
					indices.push_back(fixed_index + index + 2);

					indices.push_back(fixed_index + index - 1);
					indices.push_back(fixed_index + index + 2);
					indices.push_back(fixed_index + index + 1);
				}
				else if (index < 44) {
					indices.push_back(fixed_index + index - 1 - 8);
					indices.push_back(fixed_index + index - 8);
					indices.push_back(fixed_index + index + 2 - 8);

					indices.push_back(fixed_index + index - 1 - 8);
					indices.push_back(fixed_index + index + 2 - 8);
					indices.push_back(fixed_index + index + 1 - 8);
				}
				else {
					indices.push_back(fixed_index + index - 1 - 14);
					indices.push_back(fixed_index + index - 14);
					indices.push_back(fixed_index + index + 2 - 14);

					indices.push_back(fixed_index + index - 1 - 14);
					indices.push_back(fixed_index + index + 2 - 14);
					indices.push_back(fixed_index + index + 1 - 14);
				}
			}
			else if (index == 21 || index == 41) {
				float x_component = -0.5f - radius * cos(index * angle);
				float y_component = 0.5f + radius * sin(index * angle);
				Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				x_component = -0.5f - outter_radius * cos(index * angle);
				y_component = 0.5f + outter_radius * sin(index * angle);
				point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);
			}
		}
	}
};
struct mine_reveal_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) }
		};
		indices = {
			0, 1, 2,
			2, 3, 0
		};

		const float PI = 3.141592653f;
		float radius = 0.4f;						//size of the mine
		int slice = 16;								//how many triangles will be used
		uint32_t fixed_index = vertices.size();		//starting index for the circle objects
		float angle = PI / (float)slice;			//the angle to be incremented between triangles
		Geometry::color cl = Geometry::yellow;		//the color of triangles

		Geometry::Vertex center_circle = { { -0.5f, 0.5f, 0.02f }, Geometry::getColor(cl) };
		Geometry::Vertex first_point = { { -0.5f - radius, 0.5f, 0.02f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//yellow circle
		uint32_t max_loop_count = ((2 * slice) + (fixed_index - 2));
		for (uint32_t index = (fixed_index + 2); index < max_loop_count; index++) {
			float x_component = -0.5f - radius * cos((index - (fixed_index - 1)) * angle);
			float y_component = 0.5f + radius * sin((index - (fixed_index - 1)) * angle);
			Geometry::Vertex point = { { x_component, y_component, 0.02f }, Geometry::getColor(cl) };
			vertices.push_back(point);
			indices.push_back(fixed_index);
			indices.push_back(index - 1);
			indices.push_back(index);
		}

		indices.push_back(fixed_index);
		indices.push_back(vertices.size() - 1);
		indices.push_back(fixed_index + 1);

		//inner most black circle
		fixed_index = vertices.size();
		cl = Geometry::black;
		radius = 0.1f;

		center_circle = { { -0.5f, 0.5f, 0.04f }, Geometry::getColor(cl) };
		first_point = { { -0.5f - radius, 0.5f, 0.04f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//inner most black circle
		max_loop_count = ((2 * slice) + (fixed_index - 2));
		for (uint32_t index = (fixed_index + 2); index < max_loop_count; index++) {
			float x_component = -0.5f - radius * cos((index - (fixed_index - 1)) * angle);
			float y_component = 0.5f + radius * sin((index - (fixed_index - 1)) * angle);
			Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
			vertices.push_back(point);
			indices.push_back(fixed_index);
			indices.push_back(index - 1);
			indices.push_back(index);
		}

		indices.push_back(fixed_index);
		indices.push_back(vertices.size() - 1);
		indices.push_back(fixed_index + 1);

		//middle ring pieces
		radius = 0.2f;
		float outter_radius = 0.35f;
		fixed_index = vertices.size();

		angle /= 2.0f;

		center_circle = { { -0.5f - radius, 0.5f, 0.04f }, Geometry::getColor(cl) };
		first_point = { { -0.5f - outter_radius, 0.5f, 0.04f }, Geometry::getColor(cl) };

		vertices.push_back(center_circle);
		vertices.push_back(first_point);

		//first middle ring piece
		for (uint32_t index = 1; index < 56; index += 2) {
			if (index < 12 || (index > 22 && index < 34 || (index > 42))) {
				float x_component = -0.5f - radius * cos(index * angle);
				float y_component = 0.5f + radius * sin(index * angle);
				Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				x_component = -0.5f - outter_radius * cos(index * angle);
				y_component = 0.5f + outter_radius * sin(index * angle);
				point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				if (index < 12) {
					indices.push_back(fixed_index + index - 1);
					indices.push_back(fixed_index + index);
					indices.push_back(fixed_index + index + 2);

					indices.push_back(fixed_index + index - 1);
					indices.push_back(fixed_index + index + 2);
					indices.push_back(fixed_index + index + 1);
				}
				else if (index < 44) {
					indices.push_back(fixed_index + index - 1 - 8);
					indices.push_back(fixed_index + index - 8);
					indices.push_back(fixed_index + index + 2 - 8);

					indices.push_back(fixed_index + index - 1 - 8);
					indices.push_back(fixed_index + index + 2 - 8);
					indices.push_back(fixed_index + index + 1 - 8);
				}
				else {
					indices.push_back(fixed_index + index - 1 - 14);
					indices.push_back(fixed_index + index - 14);
					indices.push_back(fixed_index + index + 2 - 14);

					indices.push_back(fixed_index + index - 1 - 14);
					indices.push_back(fixed_index + index + 2 - 14);
					indices.push_back(fixed_index + index + 1 - 14);
				}
			}
			else if (index == 21 || index == 41) {
				float x_component = -0.5f - radius * cos(index * angle);
				float y_component = 0.5f + radius * sin(index * angle);
				Geometry::Vertex point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);

				x_component = -0.5f - outter_radius * cos(index * angle);
				y_component = 0.5f + outter_radius * sin(index * angle);
				point = { { x_component, y_component, 0.04f }, Geometry::getColor(cl) };
				vertices.push_back(point);
			}
		}
	}
};
struct adjacent_one_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.55f,  0.6f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.55f,  0.7f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.375f, 0.7f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.375f, 0.6f, 0.1f }, Geometry::getColor(Geometry::blue) },

			{ { -0.45f, 0.3f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.45f, 0.6f, 0.1f }, Geometry::getColor(Geometry::blue) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::blue) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::blue) }
		};

		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_two_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },

			{ { -0.55f, 0.525f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.65f, 0.6f,   0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.55f, 0.6f,   0.1f }, Geometry::getColor(Geometry::greenGrass) },

			{ { -0.35f, 0.3f,   0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.45f, 0.3f,   0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.45f, 0.475f, 0.1f }, Geometry::getColor(Geometry::greenGrass) },
			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::greenGrass) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_three_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkRed) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkRed) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::darkRed) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::darkRed) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::darkRed) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_four_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.55f, 0.7f, 0.1f }, Geometry::getColor(Geometry::navy) },

			{ { -0.35f, 0.5f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.45f, 0.5f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.45f, 0.7f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::navy) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::navy) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::navy) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_five_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::teal) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::teal) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::teal) },

			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.45f, 0.525f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.45f, 0.6f,   0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.35f, 0.6f,   0.1f }, Geometry::getColor(Geometry::teal) },

			{ { -0.55f, 0.3f,   0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.3f,   0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::teal) },
			{ { -0.55f, 0.475f, 0.1f }, Geometry::getColor(Geometry::teal) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_six_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::orange) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::orange) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::orange) },

			{ { -0.55f, 0.4f,   0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.4f,   0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.55f, 0.475f, 0.1f }, Geometry::getColor(Geometry::orange) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.45f, 0.3f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.45f, 0.6f, 0.1f }, Geometry::getColor(Geometry::orange) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::orange) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_seven_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::magenta) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::magenta) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::magenta) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct adjacent_eight_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::white) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::white) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.45f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.45f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::white) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct nine_tile_geometry : BaseTileGeometry {
	void initMesh() override {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::black) },

			{ { -0.35f, 0.475f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.475f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.525f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::black) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::black) },

			{ { -0.35f, 0.525f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.45f, 0.525f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.45f, 0.6f,   0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.35f, 0.6f,   0.1f }, Geometry::getColor(Geometry::black) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::black) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::black) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
struct zero_tile_geometry : BaseTileGeometry {
	void initMesh() {
		vertices = {
			{ {  0.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 0.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ { -1.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },
			{ {  0.0f, 1.0f, 0.0f }, Geometry::getColor(Geometry::gray) },

			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.65f, 0.7f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.35f, 0.7f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) },

			{ { -0.55f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.65f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.55f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.65f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.65f, 0.4f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.35f, 0.4f, 0.1f }, Geometry::getColor(Geometry::darkGray) },

			{ { -0.35f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.45f, 0.3f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.45f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) },
			{ { -0.35f, 0.6f, 0.1f }, Geometry::getColor(Geometry::darkGray) }
		};
		for (uint32_t i = 0; i < vertices.size(); i++) {
			indices.push_back(i);
			if (i % 4 == 2) {
				indices.push_back(i);
			}
			else if (i % 4 == 3) {
				indices.push_back(i - 3);
			}
		}
	}
};
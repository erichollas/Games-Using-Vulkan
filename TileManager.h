/****************************************************************************************
* TITLE:	Multi-threaded Minesweeper													*
* BY:		Eric Hollas																	*
*																						*
* FILE:		TileManager.h																*
* DETAILS:	This file defines the game logic, the geometry of the tiles, and everything *
*				in between for the minesweeper game.									*
*			Multi-threaded functions can be found in the setupMines() and revealMines() *
*				functions in the corresponding .cpp file.								*
*																						*
*****************************************************************************************/
#pragma once
#include "stdafx.h"
#include "../../VulkanRenderEngine/RenderEngine/GeometryManager.h"
#include "TileGeometry.h"

#define GAMEBOARD_ROWS 24
#define GAMEBOARD_COLUMNS 24
#define TILE_SIZE 0.0727f

#define NUM_OF_MINES 120

class TileManager {
public:
	TileManager();
	~TileManager();

	void setupMines();

	void processRightClick(float x_coord, float y_coord);

	bool processLeftClick(float x_coord, float y_coord, bool &validClick);
	bool updateTime(int time);

	Geometry::GeometryManager getGeometryInfo() const;
	std::vector<glm::mat4> getUniformMatrices() const;

private:
	//if entry is >= 10 then it is a mine, 
	//	otherwise it is equal to the number of adjacent tiles
	std::vector<std::vector<uint8_t>> gameboard_values;

	// entry == 0 for unshown, 
	//		 == 1 for flagged, 
	//		 == 2 for showing
	uint8_t gameboard_isShowing[GAMEBOARD_ROWS][GAMEBOARD_COLUMNS] = { 0 };

	uint8_t num_of_flagged;
	int previous_time = 0;

	Geometry::GeometryManager * pGeometryManager;
	std::vector<glm::mat4> uniformMatrices;


	void initGameboard();
	void revealTile(uint8_t row, uint8_t column);
	std::set<int> checkBlankPieces(uint8_t row, uint8_t column);
	void setupScoreboard();
	void revealMines();
	void updateScoreTiles();
	void updateTile(int test_num, int index);

	// the following struct row_assignment is the callable object used
	//  for multithreading purposes in the TileManager::setupMines() function
	//  because rand() is not thread safe each thread should have its own
	//  buffer of random numbers. this object will generate a row of values
	//  of type uint8_t that will be pushed into the gameboard_values 
	//	class member variable.
	struct row_assignment {
		int random_nums[100];
		std::vector<uint8_t> local_row;

		//the constructor init's the row vector with zeros and generates the
		//	buffer of random int's
		row_assignment() {
			for (uint8_t j = 0; j < 100; j++) {
				random_nums[j] = rand() % 100;
			}
			for (uint8_t j = 0; j < GAMEBOARD_COLUMNS; j++) {
				local_row.push_back(0);
			}
		}

		//this function takes in the number of mines for this row and randomly
		//	generates the mine positions in this row before pushing it into the
		//  board object which is passed by reference

		//	Note that since everything is randomly generated and the rows do 
		//  not overlap this is thread-safe
		void operator()(int numMines, std::vector<std::vector<uint8_t>> &board) {
			int mines_assigned = 0;
			int column = 0;
			int buffer_count = 0;

			//go until all mines have been assigned
			while (mines_assigned != numMines) {
				if (local_row[column] != 10) {
					//85 can be changed, it is the bottom limit to determine
					// likelyhood of mine placement
					if (random_nums[buffer_count] > 85) {
						local_row[column] = 10;
						mines_assigned++;
					}
					buffer_count++;
					if (buffer_count == 100) {
						buffer_count = 0;
					}
				}
				column++;
				if (column == GAMEBOARD_COLUMNS) {
					column = 0;
				}
			}
			board.push_back(local_row);
		}
	};
};
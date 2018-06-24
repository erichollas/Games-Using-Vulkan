/****************************************************************************************
* TITLE:	Multi-threaded Minesweeper													*
* BY:		Eric Hollas																	*
*																						*
* FILE:		TileManger.cpp																*
*																						*
* Correponds to TileManager.h (see for overall description)								*
*																						*
*	functions that contain multi-threaded functions begin on lines 41 and 670			*
*****************************************************************************************/
#include "stdafx.h"
#include "TileManager.h"


/*
*	Constructor
*/
TileManager::TileManager() {
	pGeometryManager = new Geometry::GeometryManager();
}
/*
*	Deconstructor
*/
TileManager::~TileManager() {
}

/*
* Function: setupMines
*
* Paramters: none
*
* Return Type: void
*
* Description: Several of the multithreaded algorithms are featured in this method.
*					This mehtod ensures that every member variable is initialized, 
*					then randomly generates the mine positions, clalculates the
*					tiles values as either blank, adjacent to mines, or a mine. Then
*					makes a function call to initGameboard() to initialize the geometry info.
*
*/
void TileManager::setupMines() {
	//init/re-init the class member variables
	for (int i = 0; i < GAMEBOARD_ROWS; i++) {
		for (int j = 0; j < GAMEBOARD_COLUMNS; j++) {
			gameboard_isShowing[i][j] = 0;
		}
	}
	gameboard_values.clear();
	uniformMatrices.clear();
	pGeometryManager->~GeometryManager();
	pGeometryManager = new Geometry::GeometryManager();

	//reseed the rand() function and init the local member variables
	srand(time(NULL));
	int mines_per_row[GAMEBOARD_ROWS] = { 0 };
	int mine_count = 0;
	int mine_limit = 4;
	int row = 0;
	
	//generates a buffer of random numbers to represent how many mines
	// need to be assigned to each row and ensures that correct total number
	// of mines are assigned each time.
	while (mine_count != NUM_OF_MINES) {
		if (NUM_OF_MINES - mine_count < mine_limit) {
			mines_per_row[row] += NUM_OF_MINES - mine_count;
			mine_count = NUM_OF_MINES;
		}
		else {
			int random_number = rand() % mine_limit;
			mines_per_row[row] += random_number;
			mine_count += random_number;
		}
		row++;
		if (row == GAMEBOARD_ROWS) {
			row = 0;
		}
	}

	//creates and runs the threads of the callable object defined in the class header file
	// will randomly assign mine positions throughout the game board
	//  NOTE THAT the threads run on a row by row basis so this is thread safe.
	std::vector<std::thread> threads;
	for (uint8_t i = 0; i < GAMEBOARD_ROWS; i++) {
		row_assignment row = row_assignment();
		std::thread t(row, mines_per_row[i], std::ref(gameboard_values));
		threads.push_back(std::move(t));
	}

	// since the initial geometric data for the starting board is constant, thus
	// the data in initGameboard() is mutually exclusive to the running threads and
	// is therefore thread safe
	initGameboard();

	//ensure the threads are done running by this point
	for (uint8_t i = 0; i < threads.size(); i++) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
	threads.clear();

	//the following is a lambda used to define a the method to count adjacent
	// mine tiles concurrently
	// Note that this will be thread safe as well as it runs row by row, so 
	// tiles will not be checked more than once per tile. and incrementing adjacent
	// tiles by one is not dependent on order.
	// Futhermore, if it is a mine for that tile the value will check for is
	// greater than or equal to 10, not only equal to ten, thus we can increment
	// all adjacent tiles to mines indiscreminately.
	auto counter = [](int row, std::vector<std::vector<uint8_t>> &board) {
		// go through the row tile-by-tile in this for loop
		// the if checks if the tile is a mine tile
		// the switch statement widdles down where on the board
		// the tile is to ensure no read access errors and increments
		// all adjacent tiles
		for (uint8_t column = 0; column < GAMEBOARD_COLUMNS; column++) {
			if (board[row][column] >= 10) {
				switch (row) {
				case 0:
					switch (column) {
					//upper left corner
					case 0:
						board[row + 1][column]++;
						board[row + 1][column + 1]++;
						board[row][column + 1]++;
						break;
					//upper right corner
					case GAMEBOARD_COLUMNS - 1:
						board[row + 1][column]++;
						board[row + 1][column - 1]++;
						board[row][column - 1]++;
						break;
					//upper row
					default:
						board[row + 1][column]++;
						board[row + 1][column + 1]++;
						board[row][column + 1]++;
						board[row + 1][column - 1]++;
						board[row][column - 1]++;
						break;
					}
					break;
				case GAMEBOARD_ROWS - 1:
					switch (column) {
					//bottom left corner
					case 0:
						board[row - 1][column]++;
						board[row - 1][column + 1]++;
						board[row][column + 1]++;
						break;
					//bottom right corner
					case GAMEBOARD_COLUMNS - 1:
						board[row - 1][column]++;
						board[row - 1][column - 1]++;
						board[row][column - 1]++;
						break;
					//bottom row
					default:
						board[row - 1][column - 1]++;
						board[row - 1][column]++;
						board[row - 1][column + 1]++;
						board[row][column - 1]++;
						board[row][column + 1]++;
						break;
					}
					break;
				default:
					switch (column) {
					//left column
					case 0:
						board[row - 1][column]++;
						board[row - 1][column + 1]++;
						board[row][column + 1]++;
						board[row + 1][column + 1]++;
						board[row + 1][column]++;
						break;
					//right column
					case GAMEBOARD_COLUMNS - 1:
						board[row - 1][column]++;
						board[row - 1][column - 1]++;
						board[row][column - 1]++;
						board[row + 1][column - 1]++;
						board[row + 1][column]++;
						break;
					//not on a side
					default:
						board[row - 1][column - 1]++;
						board[row][column - 1]++;
						board[row + 1][column - 1]++;
						board[row - 1][column]++;
						board[row + 1][column]++;
						board[row - 1][column + 1]++;
						board[row][column + 1]++;
						board[row + 1][column + 1]++;
						break;
					}
					break;
				}
			}
		}
	};
	// end of lambda function to be run concurrently

	//declares the threads of the previous lambda function and pushes them into the
	// vector of threads to ensure later that they will have finished by a certain point
	for (uint8_t i = 0; i < GAMEBOARD_ROWS; i++) {
		std::thread t(counter, i, std::ref(gameboard_values));
		threads.push_back(std::move(t));
	}

	//the following makes a call to a function that sets up the feedback info at the top of the screen
	setupScoreboard();
	num_of_flagged = NUM_OF_MINES;
	updateScoreTiles();

	//ensures that the threads finish here (may not be necessary since it is the end of the file, but
	//  adding it here in case something needs to be added after this later
	for (uint8_t i = 0; i < GAMEBOARD_ROWS; i++) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
}
/*
* Function: processRightClick
*
* Paramters: float x_coord,
*			 float y_coord
*
* Return Type: void
*
* Description: Processes right clicks based on the parameters passed
*					calculates the tile clicked, if the tile has already
*					been clicked or flagged and updates the mines score
*					at the top of the screen accordingly
*
*/
void TileManager::processRightClick(float x_coord, float y_coord) {
	// the if ensures the click happened in the playable area
	if (x_coord < 0.95f && y_coord < (0.78 + TILE_SIZE) && x_coord >(0.95f - (GAMEBOARD_COLUMNS + 1) * (TILE_SIZE + 0.005f)) && y_coord >(0.78f - GAMEBOARD_ROWS * (TILE_SIZE + 0.005f))) {
		//the row and column variable as well as the following while loops determine
		// the tile that was clicked
		uint32_t row = 0;
		uint32_t column = 0;
		while (x_coord < 0.95f - (column + 1) * (TILE_SIZE + 0.005f)) {
			column++;
		}
		while (y_coord < (0.78f + TILE_SIZE + 0.005f) - (row + 1) * (TILE_SIZE + 0.005f)) {
			row++;
		}
		//this if-else statement determines if the tile has been flagged or not and ignores already revealed tiles
		// the inside code of each case is similar just whether to make the tile geometry have a flag or not and
		// whether the mine score at the top of the screen should go down or not
		if (gameboard_isShowing[row][column] == 1) {
			if (num_of_flagged < NUM_OF_MINES) {
				num_of_flagged++;
				updateScoreTiles();
			}
			gameboard_isShowing[row][column] = 0;

			unshown_tile_geometry tile;
			tile.initMesh();
			pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
		}
		else if (gameboard_isShowing[row][column] == 0) {
			if (num_of_flagged > 0) {
				num_of_flagged--;
				updateScoreTiles();
			}
			gameboard_isShowing[row][column] = 1;

			flagged_tile_geometry tile;
			tile.initMesh();
			pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
		}
	}
}
/*
* Function: processLeftClick
*
* Paramters: float x_coord, 
*			 float y_coord, 
*			 bool &validClick
*
* Return Type: bool
*
* Description: Processes the left click based on the float parameters passed.
*					Returns true if the user lost and clicked on a mine, otherwise
*					returns false. validClick will be true if a valid tile was clicked
*					otherwise it will be false. validClick will be used to determine
*					if all the tiles have been clicked and the game is over.
*
*/
bool TileManager::processLeftClick(float x_coord, float y_coord, bool &validClick) {
	validClick = false;
	//this if determines if the click happened on the playbale area
	if (x_coord < 0.95f && y_coord < (0.78 + TILE_SIZE) && x_coord > (0.95f - (GAMEBOARD_COLUMNS + 1) * (TILE_SIZE + 0.005f)) && y_coord > (0.78f - GAMEBOARD_ROWS * (TILE_SIZE + 0.005f))) {
		//row and column and the two following while loops calculate the tile clicked
		uint32_t row = 0;
		uint32_t column = 0;
		while (x_coord < 0.95f - (column + 1) * (TILE_SIZE + 0.005f)) {
			column++;
		}
		while (y_coord < (0.78f + TILE_SIZE + 0.005f) - (row + 1) * (TILE_SIZE + 0.005f)) {
			row++;
		}

		//checks if tile is unclicked
		if (gameboard_isShowing[row][column] == 0) {
			validClick = true;
			// the if-else determines if the tile is unshown, a mine, or a blank space
			if (gameboard_values[row][column] == 0) {
				// collects all adjacent blank tiles and tiles adjacent to blank
				// tiles to be revealed from this click. The set pieces will contain
				// said pieces and will not have duplicates since it is of type std::set
				std::set<int> pieces = checkBlankPieces(row, column);
				std::set<int>::iterator it = pieces.begin();
				//reveals all the tiles in pieces set
				while (it != pieces.end()) {
					int num = *it;
					int r = num / GAMEBOARD_COLUMNS;
					int c = num % GAMEBOARD_COLUMNS;
					
					//calls revealTile if it is not showing to set the new geometric info
					if (gameboard_isShowing[r][c] == 0) {
						gameboard_isShowing[r][c] = 2;
						revealTile(r, c);
					}
					it++;
				}
			}
			else if (gameboard_values[row][column] < 10) {
				//reveals the one tile that is a number tile by calling revealTile()
				gameboard_isShowing[row][column] = 2;
				revealTile(row, column);
			}
			else {
				//reveals all the mines by calling revealMines() and revealTile()
				// returns true because user clicked a mine
				revealMines();
				gameboard_isShowing[row][column] = 2;
				revealTile(row, column);
				return true;
			}
		}
	}
	return false;
}
/*
* Function: updateTime
*
* Paramters: int time
*
* Return Type: bool
*
* Description: Updates the timer in the top right of the screen if a second has passed.
*					If the size of the board changes the indices passed to updateTile 
*					need to changed accordingly.
*				
*
*/
bool TileManager::updateTime(int time) {
	if (previous_time != time) {
		int hundreds_place = time - (time % 100);
		int tens_place = (time - hundreds_place);
		int ones_place = time % 10;
		tens_place -= ones_place;
		hundreds_place /= 100;
		tens_place /= 10;

		//the indices in the GeometryManager are 579, 580, and 581 for the timer square
		updateTile(hundreds_place, 579);
		updateTile(tens_place, 580);
		updateTile(ones_place, 581);

		return true;
	}
	return false;
}

/*
* The next two functions are generic accessor functions used to passed 
*		render data to the render engine in the window class
*
*/
Geometry::GeometryManager TileManager::getGeometryInfo() const {
	return *pGeometryManager;
}

std::vector<glm::mat4> TileManager::getUniformMatrices() const {
	return uniformMatrices;
}

//************************************************
//END OF PUBLIC METHODS, START OF PRIVATE METHODS*
//************************************************

/*
* Function: initGameboard
*
* Paramters: none
*
* Return Type: void
*
* Description: Initializes the gameboard tiles to have the unshown_tile_geometry
*				and sets each corresponding uniform matrix to position each tile evenly
*
*/
void TileManager::initGameboard() {
	for (uint32_t i = 0; i < GAMEBOARD_ROWS; i++) {
		std::vector<uint8_t> row = gameboard_values[i];
		for (uint32_t j = 0; j < GAMEBOARD_COLUMNS; j++) {
			unshown_tile_geometry tile;
			tile.initMesh();
			pGeometryManager->addObject(tile.vertices, tile.indices);


			float xPos = 0.95f - j * (TILE_SIZE + 0.005f);
			float yPos = 0.78f - i * (TILE_SIZE + 0.005f);
			glm::mat4 newMatrix = glm::mat4{ TILE_SIZE,	0.0f,		0.0f,		0.0f,
				0.0f,		TILE_SIZE,	0.0f,		0.0f,
				0.0f,		0.0f,		TILE_SIZE,	0.0f,
				xPos,		yPos,		0.0f,		1.0f
			};
			uniformMatrices.push_back(newMatrix);
		}
	}
}
/*
* Function: revealTile
*
* Paramters: uint8_t row, 
*			 uint8_t column
*
* Return Type: void
*
* Description: Uses the parameters to calculate the position of the tile in the pGeometryManager
*					and to find the correct type of tile to set this tile based on the current value
*					in the gameboard_values matrix.
*					Also, here is used an if-else instead of switch because the new geometry must be 
*					defined locally in each case to update the new tile geometry, which is not allowed
*					in a switch statement.
*
*/
void TileManager::revealTile(uint8_t row, uint8_t column) {
	if (gameboard_values[row][column] == 0) {
		blank_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 1) {
		adjacent_one_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 2) {
		adjacent_two_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 3) {
		adjacent_three_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 4) {
		adjacent_four_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 5) {
		adjacent_five_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 6) {
		adjacent_six_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 7) {
		adjacent_seven_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else if (gameboard_values[row][column] == 8) {
		adjacent_eight_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
	else {
		mine_hit_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject((column + row * GAMEBOARD_COLUMNS), tile.vertices, tile.indices);
	}
}
/*
* Function: checkBlankPieces
*
* Paramters: uint8_t row, 
*			 uint8_t column
*
* Return Type: std::set<int>
*
* Description: Uses the passed paramters to find all the adjacent blank tiles
*				and all tiles adjacent to all the blank tiles to be added to 
*				std::set<int> object and then returns the set. Note that a
*				set object is best here because duplicates would devastate performance
*
*/
std::set<int> TileManager::checkBlankPieces(uint8_t row, uint8_t column) {
	std::set<int> blankTiles;
	blankTiles.insert(column + row * GAMEBOARD_COLUMNS);

	//the flag will be true when the size of the set is unchanged for one loop cycle
	//  meaning all the tiles we need are added to the set
	bool isDone = false;
	while (!isDone) {
		int set_size = blankTiles.size();
		std::set<int>::iterator it = blankTiles.begin();
		while (it != blankTiles.end()) {
			int num = *it;
			int r = num / GAMEBOARD_COLUMNS;
			int c = num % GAMEBOARD_COLUMNS;

			//the nested if/if-else statement adds the appropriate tiles to the set
			// base on if they are showing, flagged, or valid tiles based on board position
			// (to ensure no read access errors)
			if (gameboard_values[r][c] == 0 && gameboard_isShowing[r][c] == 0) {
				if (r + 1 < GAMEBOARD_ROWS && c + 1 < GAMEBOARD_COLUMNS) {
					blankTiles.insert((c + 1) + r * GAMEBOARD_COLUMNS);
					blankTiles.insert((c + 1) + (r + 1) * GAMEBOARD_COLUMNS);
					blankTiles.insert(c + (r + 1) * GAMEBOARD_COLUMNS);
				}
				else if (r + 1 < GAMEBOARD_ROWS) {
					blankTiles.insert(c + (r + 1) * GAMEBOARD_COLUMNS);
				}
				else if (c + 1 < GAMEBOARD_COLUMNS) {
					blankTiles.insert((c + 1) + r * GAMEBOARD_COLUMNS);
				}

				if (r - 1 >= 0 && c - 1 >= 0) {
					blankTiles.insert((c - 1) + r * GAMEBOARD_COLUMNS);
					blankTiles.insert((c - 1) + (r - 1) * GAMEBOARD_COLUMNS);
					blankTiles.insert(c + (r - 1) * GAMEBOARD_COLUMNS);
				}
				else if (r - 1 >= 0) {
					blankTiles.insert(c + (r - 1) * GAMEBOARD_COLUMNS);
				}
				else if (c - 1 >= 0) {
					blankTiles.insert((c - 1) + r * GAMEBOARD_COLUMNS);
				}

				if (r - 1 >= 0 && c + 1 < GAMEBOARD_COLUMNS) {
					blankTiles.insert((c + 1) + (r - 1) * GAMEBOARD_COLUMNS);
					blankTiles.insert(c + (r - 1) * GAMEBOARD_COLUMNS);
				}
				if (r + 1 < GAMEBOARD_ROWS && c - 1 >= 0) {
					blankTiles.insert((c - 1) + (r + 1) * GAMEBOARD_COLUMNS);
					blankTiles.insert(c + (r + 1) * GAMEBOARD_COLUMNS);
				}
			}
			it++;
		}

		if (set_size == blankTiles.size()) {
			isDone = true;
		}
	}
	return blankTiles;
}
/*
* Function: setupScoreboard
*
* Paramters: none
*
* Return Type: void
*
* Description: Initializes the the tile geometry and the matirices for the tiles used
*					at the top left and top right of the screen that display the mines flagged
*					and the time elapsed per game.
*				Note that: if the size of the board changes the indices for the tiles within the
*					pGeometryManager object will need to be changed accordingly.
*
*/
void TileManager::setupScoreboard() {
	adjacent_one_tile_geometry one_tile;
	one_tile.initMesh();
	adjacent_two_tile_geometry two_tile;
	two_tile.initMesh();
	adjacent_five_tile_geometry five_tile;
	five_tile.initMesh();

	zero_tile_geometry zero_tile;
	zero_tile.initMesh();


	pGeometryManager->addObject(one_tile.vertices, one_tile.indices);		//index == 576
	pGeometryManager->addObject(two_tile.vertices, two_tile.indices);		//index == 577
	pGeometryManager->addObject(five_tile.vertices, five_tile.indices);		//index == 578

	pGeometryManager->addObject(zero_tile.vertices, zero_tile.indices);		//index == 579
	pGeometryManager->addObject(zero_tile.vertices, zero_tile.indices);		//index == 580
	pGeometryManager->addObject(zero_tile.vertices, zero_tile.indices);		//index == 581

	float xPosition = 0.85f;
	float yPosition = 0.90f;
	glm::mat4 one_matrix = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(one_matrix);

	xPosition -= (TILE_SIZE + 0.005f);
	glm::mat4 two_matrix = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(two_matrix);

	xPosition -= (TILE_SIZE + 0.005f);
	glm::mat4 five_matrix = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(five_matrix);

	xPosition -= (18 * TILE_SIZE);
	glm::mat4 zero_matrix1 = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(zero_matrix1);

	xPosition -= (TILE_SIZE + 0.005f);
	glm::mat4 zero_matrix2 = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(zero_matrix2);

	xPosition -= (TILE_SIZE + 0.005f);
	glm::mat4 zero_matrix3 = glm::mat4{	TILE_SIZE,	0.0f,		0.0f,		0.0f,
										0.0f,		TILE_SIZE,	0.0f,		0.0f,
										0.0f,		0.0f,		TILE_SIZE,	0.0f,
										xPosition,	yPosition,	0.0f,		1.0f
									};
	uniformMatrices.push_back(zero_matrix3);
}
/*
* Function: revealMines
*
* Paramters: none
*
* Return Type: void
*
* Description: Searches the gameboard for all the mines and displays the mines with
*					gray backgrounds.
*				Declares and defines the lambda reveal that searches the board row by
*					row and logs the mines found into a std::deque object. This lambda
*					will be the threaded function to search the gameboard row by row.
*					Since the board's rows do not overlap, this will be thread safe.
*
*/
void TileManager::revealMines() {
	//defines the lambda as described in the function's description
	auto reveal = [](std::deque<int> &pieces, int row, std::vector<std::vector<uint8_t>> board) {
		for (int column = 0; column < GAMEBOARD_COLUMNS; column++) {
			if (board[row][column] > 9) {
				pieces.push_back(column + row * GAMEBOARD_COLUMNS);
			}
		}
	};
	//pieces will be used to log all the mine tiles to be revealed
	std::deque<int> pieces;

	// the following creates, runs and ensures the threads using the reveal lambda have finished
	//   before the final while loop goes through each tile in the deque to reveal the found mines
	std::vector<std::thread> threads;
	for (uint8_t row = 0; row < GAMEBOARD_ROWS; row++) {
		std::thread t(reveal, std::ref(pieces), row, gameboard_values);
		threads.push_back(std::move(t));
	}
	for (uint8_t i = 0; i < GAMEBOARD_ROWS; i++) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
	while (!pieces.empty()) {
		mine_reveal_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(pieces.front(), tile.vertices, tile.indices);
		pieces.pop_front();
	}
}
/*
* Function: updateScoreTiles
*
* Paramters: none
*
* Return Type: void
*
* Description: This is to be called when a tile is flagged or un-flagged (right-clicked tile)
*					This just updates the tiles at the top left of the screen to show how many 
*					mines are left.
*				Note that: if the size of the board is changed the values for the indices of the
*					tiles in the pGeometryManager object must be changed accordingly.
*
*/
void TileManager::updateScoreTiles() {
	int flagged = num_of_flagged;

	//mine tracking tiles are indexed 576, 577, and 578 in the geometry manager
	if (flagged >= 100) {
		adjacent_one_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(576, tile.vertices, tile.indices);
		flagged -= 100;
	}
	else {
		zero_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(576, tile.vertices, tile.indices);
	}
	int ones_place = flagged % 10;
	int tens_place = flagged - ones_place;
	tens_place /= 10;

	updateTile(tens_place, 577);
	updateTile(ones_place, 578);
}
/*
* Function: updateTile
*
* Paramters: int test_num,
*			 int index
*
* Return Type: void
*
* Description: Is essentially an overload of the revealTile() method just the tiles
*					do not correspond to the gameboard_values matrix. Thus the parameters
*					and the new number to indicate the new geometry info for the tile and 
*					index param is the indice of the tile in the pGeometryManager object.
*
*/
void TileManager::updateTile(int test_num, int index) {
	if (test_num == 0) {
		zero_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 1) {
		adjacent_one_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 2) {
		adjacent_two_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 3) {
		adjacent_three_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 4) {
		adjacent_four_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 5) {
		adjacent_five_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 6) {
		adjacent_six_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 7) {
		adjacent_seven_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 8) {
		adjacent_eight_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
	else if (test_num == 9) {
		nine_tile_geometry tile;
		tile.initMesh();
		pGeometryManager->updateObject(index, tile.vertices, tile.indices);
	}
}
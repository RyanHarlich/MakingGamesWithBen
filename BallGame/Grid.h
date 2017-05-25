#pragma once

#include "Ball.h"
#include <vector>

struct Cell {
	// Moving a vector of balls around is alot 32 bytes, pointers are typically only 8 bytes
	std::vector<Ball*> balls;
};




class Grid
{
public:
	/* NEW */
	friend class BallController;

	Grid(int width, int height, int cellSize);
	~Grid();

	// Adds a ball and determines which cell it belongs to
	void addBall(Ball* ball);

	/* NEW: overloaded addBall and adds ball to specified cell */
	void addBall(Ball* ball, Cell* cell);



	// Get cell based on cell coordinates, get cell based on position of cell
	/* NEW: changed to return a pointer instead of a reference */
	Cell* getCell(int x, int y);
	// Gets cell based on window coordinates, get cell based on position of ball
	/* NEW: changed to return a pointer instead of a reference */
	Cell* getCell(const glm::vec2& pos);

	/* NEW */
	void removeBallFromCell(Ball* ball);


	//2D grid array: treat a one-dimensional vector as a two-dimensional vector for cache friendliness, a two-dimensional vector jumps around in memory alot shown in .cpp file
	std::vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells; // width in cells
	int m_numYCells; // height in cells

};


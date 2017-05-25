#include "Grid.h"



Grid::Grid(int width, int height, int cellSize) : 
	m_width(width), 
	m_height(height), 
	m_cellSize(cellSize) {
	m_numXCells = ceil((float)m_width / m_cellSize); // in case grid is smaller than size of screen
	m_numYCells = ceil((float)m_height / m_cellSize); // make extra cells with ceiling function
													// illustrated in picture

	//int myArray[3][3]; this is actually stored as a one-dimensional array int myArray[9];
	
	// Allocate all the cells
	m_cells.resize(m_numYCells * m_numXCells);
}


Grid::~Grid()
{
}

void Grid::addBall(Ball* ball)
{
	Cell& cell = getCell(ball->position);
	cell.balls.push_back(ball);

	/* NEW TO BE CONTINUED FOR PART 2: cannot have a pointer to a reference's address */
	ball->ownerCell = &cell;

}





// Get cell based on cell coordinates, get cell based on position of cell
Cell& Grid::getCell(int x, int y){
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	// 1D array treated as a 2D array spot [0][1] is 1 and [1][1] is 11 if 10x10
	//return m_cells[(y - 1) * m_numXCells + (x - 1)];
	return m_cells[y * m_numXCells + m_numXCells];
}





// Gets cell based on window coordinates, get cell based on position of ball
Cell& Grid::getCell(const glm::vec2 & pos) {

	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	return getCell(cellX, cellY);
}







#include "Grid.h"



Grid::Grid(int width, int height, int cellSize) : 
	m_width(width), 
	m_height(height), 
	m_cellSize(cellSize) {
	m_numXCells = (int)ceil((float)m_width / m_cellSize); // in case grid is smaller than size of screen
	m_numYCells = (int)ceil((float)m_height / m_cellSize); // make extra cells with ceiling function
													// illustrated in picture

	//int myArray[3][3]; this is actually stored as a one-dimensional array int myArray[9];
	
	// Allocate all the cells
	//m_cells.resize(m_numYCells * m_numXCells);

	/* NEW: this will boost fps, but will improve by reserving memory so it does not need to create it later during the program so each cell has a reserved memory space of 20 balls during any time throughout the program */
	const int BALLS_TO_RESERVE = 20;
	m_cells.resize(m_numYCells * m_numXCells);
	for (size_t i = 0; i < m_cells.size(); ++i) {
		m_cells[i].balls.reserve(BALLS_TO_RESERVE);
	}

}






Grid::~Grid()
{
}







void Grid::addBall(Ball* ball)
{
	/* NEW: changed to return a pointer */
	//Cell& cell = getCell(ball->position);
	Cell* cell = getCell(ball->position);

	/* NEW: changed to a pointer type */
	//cell.balls.push_back(ball);
	cell->balls.push_back(ball);

	/* NEW CONTINUED FOR PART 2: cannot have a pointer to a reference's address */
	//ball->ownerCell = &cell;
	ball->ownerCell = cell;


	/* NEW: new emplaced ball vector index equals the last spot in the  */
	ball->cellVectorIndex = cell->balls.size() - 1;



	// Could replace the three lines above and call addBall(ball, cell) overloaded method, but that "could" add some overhead to the program
}







void Grid::addBall(Ball* ball, Cell* cell){
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}







// Get cell based on cell coordinates, get cell based on position of cell
/* NEW: changed to return a pointer instead of a reference */
Cell* Grid::getCell(int x, int y){
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	// 1D array treated as a 2D array spot [0][1] is 1 and [1][1] is 11 if 10x10
	//return m_cells[(y - 1) * m_numXCells + (x - 1)];
	/* NEW: changed to return a pointer instead of a reference, also changed the math too right math */
	//return m_cells[y * m_numXCells + m_numXCells];
	return &m_cells[y * m_numXCells + x];
}





// Gets cell based on window coordinates, get cell based on position of ball
/* NEW: changed to return a pointer instead of a reference */
Cell* Grid::getCell(const glm::vec2 & pos) {

	int cellX = (int)(pos.x / m_cellSize);
	int cellY = (int)(pos.y / m_cellSize);

	/* NEW: changed to return a pointer instead of a reference */
	//return getCell(cellX, cellY);
	return getCell(cellX, cellY);
}





/* NEW */
void Grid::removeBallFromCell(Ball* ball) {

	std::vector<Ball*>& balls = ball->ownerCell->balls;
	// Normal vector swap
	balls[ball->cellVectorIndex] = balls.back();
	balls.pop_back();
	// Update vector index
	if (ball->cellVectorIndex < (int)balls.size()) {
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	// Set the index of ball to -1
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;

}







#pragma once

#include <vector>

#include "Ball.h"


class Grid; // class forward declarations (for pointers only or reference) are to improve compile times because every file that includes ball.h will have to include grid.h if included, this forward declaration avoids that behaviour

enum class GravityDirection {NONE, LEFT, UP, RIGHT, DOWN};

class BallController
{
public:
	/// Updates the balls
	void updateBalls(std::vector <Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY);
	///Some simple event functions
	void onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY);
	void onMouseUp(std::vector <Ball>& balls);
	void onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY);
	void setGravityDirection(GravityDirection dir) { m_gravityDirection = dir; }
private:


	void updateCollision(Grid* grid);
	void checkCollision(Ball* ball, std::vector<Ball*>& ballsToCheck, int startingIndex);


	/// Checks collision between two balls
	void checkCollision(Ball& b1, Ball& b2);

	/// Returns true if the mouse is hovering over a ball
	bool isMouseOnBall(Ball& b, float mouseX, float mouseY);
	glm::vec2 getGravityAccel();

	int m_grabbedBall = -1; ///< The ball we are currently grabbing on to
	glm::vec2 m_prevPos = glm::vec2(0.0f); ///< Previous position of the grabbed ball
	glm::vec2 m_grabOffSet = glm::vec2(0.0f); ///< Offset of the cursor on the selected ball

	GravityDirection m_gravityDirection = GravityDirection::NONE;
};


#pragma once

#include <vector>
#include <memory>

#include "Ball.h"
#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

#include <Bengine/Window.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>
#include <Bengine/Timing.h>
#include <Bengine/AudioEngine.h>


enum class GameState {
	RUNNING,
	EXIT
};

/* NEW: with spatial partition running at less than 50 fps with size at 128 at 4000 balls (option 1) */
//const int CELL_SIZE = 128;
//const int CELL_SIZE = 32; // this runs at 60fps with 10,000 balls (option 1), (option 2) with 40,000 balls, smaller balls, runs at 2fps
const int CELL_SIZE = 12; // the smaller the cells the more cells to iterate through (option 1, runs at 60fps with 20,000 balls with the same result if CELL_SIZE is 32), (option 2) with 40,000 balls, smaller balls, runs at 10fps


class MainGame
{
public:
	void run();

private:

	void init();


	void initBalls();


	void gameLoop();


	void processInput();

	
	void update(float deltaTime);


	void draw();


	void drawHud();




private:
	Bengine::Window m_window;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::InputManager m_inputManager;
	Bengine::Camera2D m_camera;
	Bengine::SpriteBatch m_spriteBatch;
	Bengine::FpsLimiter m_fpsLimiter;
	Bengine::AudioEngine m_audioEngine;

	BallRenderer m_ballRenderer;
	BallController m_ballController;
	std::vector<Ball> m_balls; ///< if balls were delete should be pointers, but this is more cache friendly if not
	std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

	int m_screenWidth = 1024;
	int m_screenHeight = 768;
	float m_fps;
	std::unique_ptr<Bengine::SpriteFont> m_spriteFont;
	GameState m_gameState;

};


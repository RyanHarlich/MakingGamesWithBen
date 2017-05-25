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


const int CELL_SIZE = 12;


class MainGame
{
public:
	~MainGame();

	void run();

private:
	void init();
	void initRenderers();
	void initBalls();
	void gameLoop();
	void processInput();
	void update(float deltaTime);
	void draw();
	void drawHud();



	Bengine::Window m_window;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::InputManager m_inputManager;
	Bengine::Camera2D m_camera;
	Bengine::SpriteBatch m_spriteBatch;
	Bengine::FpsLimiter m_fpsLimiter;
	Bengine::AudioEngine m_audioEngine;


	BallController m_ballController;
	std::vector<Ball> m_balls; ///< if balls were delete should be pointers, but this is more cache friendly if not
	std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision
	std::vector<BallRenderer*> m_ballRenderers;
	int m_currentRenderer = 0;
	

	int m_screenWidth = 1024;
	int m_screenHeight = 768;
	float m_fps;
	std::unique_ptr<Bengine::SpriteFont> m_spriteFont;
	GameState m_gameState;

};


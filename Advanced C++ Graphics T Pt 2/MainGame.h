#pragma once

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

#include <Bengine/Window.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>



class Zombie;

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:
	
	void initSystems();

	
	void initLevel();

	
	void initShaders();

	
	void gameLoop();


	void updateAgents(float deltaTime);


	void updateBullets(float deltaTime);

	
	void checkVictory();


	void processInput();

	
	void drawGame();


	void drawHud();


private:	
	Bengine::Window m_window;				
	Bengine::GLSLProgram m_textureProgram;	
	Bengine::InputManager m_inputManager;	
	Bengine::Camera2D m_camera;
	Bengine::Camera2D m_hudCamera;
	Bengine::SpriteBatch m_agentSpriteBatch; 
	Bengine::SpriteBatch m_hudSpriteBatch;
	std::vector<Level*> m_levels;

	/* NEW: header initialization, not new variables only new initialization with an m in front */
	int m_screenWidth = 1024;
	int m_screenHeight = 768;


	float m_fps;
	int m_currentLevel;
	Player* m_player;
	std::vector<Human*> m_humans; 
	std::vector<Zombie*> m_zombies; 
	std::vector<Bullet> m_bullets;
	int m_numHumansKilled; 
	int m_numZombiesKilled; 
	Bengine::SpriteFont* m_spriteFont;
	GameState m_gameState;
	


};


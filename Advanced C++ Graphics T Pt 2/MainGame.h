#pragma once

#include "Level.h"
#include "Player.h"
#include "Bullet.h"

#include <Bengine/Window.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>

/* NEW */
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


	/* NEW: Draws the HUD */
	void drawHud();


private:	
	Bengine::Window _window;				
	Bengine::GLSLProgram _textureProgram;	
	Bengine::InputManager _inputManager;	
	Bengine::Camera2D _camera;

	/* NEW */
	Bengine::Camera2D _hudCamera;

	Bengine::SpriteBatch _agentSpriteBatch; 

	/* NEW */
	Bengine::SpriteBatch _hudSpriteBatch;

	std::vector<Level*> _levels; 
	int _screenWidth, _screenHeight;
	float _fps;
	int _currentLevel;
	Player* _player;
	std::vector<Human*> _humans; 
	std::vector<Zombie*> _zombies; 
	std::vector<Bullet> _bullets;
	int _numHumansKilled; 
	int _numZombiesKilled; 

	/* NEW */
	Bengine::SpriteFont* _spriteFont;

	GameState _gameState;
	


};


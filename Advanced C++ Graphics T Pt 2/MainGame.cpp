#include <random>
#include <ctime>
#include <iostream>
#include <algorithm> /* min(,) */

#include "Gun.h"
#include "MainGame.h"
#include "Zombie.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;



MainGame::MainGame() :
	_screenWidth(1024),
	_screenHeight(768),
	_fps(0),
	_player(nullptr),
	_gameState(GameState::PLAY),
	_numHumansKilled(0),
	_numZombiesKilled(0)
{
}




MainGame::~MainGame()
{
	for (unsigned int i = 0; i < _levels.size(); ++i) {
		delete _levels[i];
	}
	for (unsigned int i = 0; i < _humans.size(); ++i) {
		delete _humans[i];
	}
	for (unsigned int i = 0; i < _zombies.size(); ++i) {
		delete _zombies[i];
	}
}



void MainGame::run() {

	initSystems();
	initLevel();
	gameLoop();

}




void MainGame::initSystems() {

	Bengine::init();
	_window.create("ZombieGame", _screenWidth, _screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initShaders();
	_agentSpriteBatch.init();
	_camera.init(_screenWidth, _screenHeight);


}




void MainGame::initLevel() {
	

	_levels.push_back(new Level("Levels/level1.txt"));


	_currentLevel = 0;

	_player = new Player();
	_player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);

	_humans.push_back(_player); 



	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2); 
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);




	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); ++i) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPositions();
	for (unsigned int i = 0; i < zombiePositions.size(); ++i) {
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}
}


void MainGame::initShaders() {
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("fragmentPosition");
	_textureProgram.addAttribute("fragmentColor");
	_textureProgram.addAttribute("fragmentUV");
	_textureProgram.linkShaders();

}


void MainGame::gameLoop() {

	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(20.0f);


	const float CAMERA_SCALE = 1.0f / 4.0f;
	_camera.setScale(CAMERA_SCALE);


	const float MS_PER_SECOND = 1000; 
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; 
	const float MAX_DELTA_TIME = 1.0f; 
	float previousTicks = (float)SDL_GetTicks();




	while (_gameState == GameState::PLAY) {
		fpsLimiter.begin();


		float newTicks = (float)SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;


		checkVictory();

		_inputManager.update();

		processInput();


		int i = 0;
		while (totalDeltaTime > 0.0f && i++ < MAX_PHYSICS_STEPS) {

			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			updateAgents(deltaTime);
			updateBullets(deltaTime);


			totalDeltaTime -= deltaTime;
		}

		_camera.setPosition(_player->getPosition());


		_camera.update();
		drawGame();

		_fps = fpsLimiter.end();


		std::cout << _fps << std::endl;
	}
}




void MainGame::updateAgents(float deltaTime) {
	
	for (unsigned int i = 0; i < _humans.size(); ++i) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}


	for (unsigned int i = 0; i < _zombies.size(); ++i) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies, deltaTime);
	}


	for (unsigned int i = 0; i < _zombies.size(); ++i) {	
		for (unsigned int j = i + 1; j < _zombies.size(); ++j) { 
			_zombies[i]->collideWithAgent(_zombies[j]);
		}	

		for (unsigned int j = 1; j < _humans.size(); ++j) { 	
			if (_zombies[i]->collideWithAgent(_humans[j])) {
		
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
			
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}

	
		if (_zombies[i]->collideWithAgent(_player)) {
			std::cout << "YOU LOSE" << std::endl;
			std::cout << "Press any key to continue...\n";
			char tmp;
			std::cin >> tmp;
			_gameState = GameState::EXIT;
		}
	}


	
	for (unsigned int i = 0; i < _humans.size(); ++i) {	
		for (unsigned int j = i + 1; j < _humans.size(); ++j) { 
			_humans[i]->collideWithAgent(_humans[j]);
		}
	}



	const float BULLET_SPEED = 20.0f;


	_player->addGun(new Gun("Magnum", 10, 1, 0.06f, 30, BULLET_SPEED));
	_player->addGun(new Gun("Shotgun", 30, 12, 0.7f, 4, BULLET_SPEED));
	_player->addGun(new Gun("MP5", 2, 1, 0.2f, 20, BULLET_SPEED)); 

}





void MainGame::updateBullets(float deltaTime) {

	for (unsigned int i = 0; i < _bullets.size();) {

		if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			++i;
		}
	}

	bool wasBulletRemoved;


	for (unsigned int i = 0; i < _bullets.size(); ++i) {

		wasBulletRemoved = false;

		for (unsigned int j = 0; j < _zombies.size();) {	
			if (_bullets[i].collideWithAgent(_zombies[j])) {		
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {		
					delete _zombies[j];
					_zombies[j] = _zombies.back();
					_zombies.pop_back();
					++_numZombiesKilled;
				}
				else {
					++j;
				}

			
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				--i; 
		
				break;			
			}
			else {
				++j;
			}
		}




	
		if (wasBulletRemoved == false) {
			for (unsigned int j = 1; j < _humans.size();) { 		
				if (_bullets[i].collideWithAgent(_humans[j])) {			
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {

						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
						++_numHumansKilled;
					}
					else {
						++j;
					}

					
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					wasBulletRemoved = true;
					--i; 
					break;
				}
				else {
					++j;
				}
			}
		}
	}


}



void MainGame::checkVictory() {

	if (_zombies.empty()) {
		printf("*** You win! ***\nYou killed %d humans and %d zombies. There are %d/%d civilians remaining\n",
			_numHumansKilled,
			_numZombiesKilled,
			_humans.size() - 1, 
			_levels[_currentLevel]->getNumHumans());
		std::cout << "Press any key to continue...\n";
		char tmp;
		std::cin >> tmp;
		_gameState = GameState::EXIT;
	}
}




void MainGame::processInput() {

	SDL_Event evnt;

	
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}






void MainGame::drawGame() {
	glClearDepth(1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0); 
	
	glUniform1i(_textureProgram.getUniformLocation("mySampler"), 0); 

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]); 


	_levels[_currentLevel]->draw();


	_agentSpriteBatch.begin();
	for (unsigned int i = 0; i < _humans.size(); ++i) {
		_humans[i]->draw(_agentSpriteBatch);
	}
	for (unsigned int i = 0; i < _zombies.size(); ++i) {
		_zombies[i]->draw(_agentSpriteBatch);
	}
	for (unsigned int i = 0; i < _bullets.size(); ++i) {
		_bullets[i].draw(_agentSpriteBatch);
	}
	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();



	_textureProgram.unuse();
	_window.swapBuffer();
}

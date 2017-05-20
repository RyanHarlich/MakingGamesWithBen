#include "Player.h"
#include "Gun.h"

#include <SDL/SDL.h>



Player::Player() : 
	_currentGunIndex(-1)
{
}




Player::~Player()
{
}



void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets) {
	_speed = speed;
	_position = pos;
	_inputManager = inputManager;
	_bullets = bullets;
	_camera = camera;
	_color = Bengine::ColorRGBA8(0, 0, 185, 255);
	_health = 150;
}





void Player::addGun(Gun* gun) {

	_guns.push_back(gun);
	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}



void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime) {



	if (_inputManager->isKeyDown(SDLK_w) || _inputManager->isKeyDown(SDLK_UP)) {
		_position.y += _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_s) || _inputManager->isKeyDown(SDLK_DOWN)) {
		_position.y -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT)) {
		_position.x -= _speed * deltaTime;
	}
	if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT)) {
		_position.x += _speed * deltaTime;
	}




	if (_inputManager->isKeyDown(SDLK_1) && _guns.size() >= 0) {
		_currentGunIndex = 0;
	}
	if (_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 0) {
		_currentGunIndex = 1;
	}
	if (_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 0) {
		_currentGunIndex = 2;
	}





	if (_currentGunIndex != -1) {

		glm::vec2 mouseCoords = _inputManager->getMouseCoords();
		mouseCoords = _camera->convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		_guns[_currentGunIndex]->update(_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			centerPosition,
			direction,
			// derefernce operator
			*_bullets, 
			deltaTime);
	}


	collideWithLevel(levelData);

}

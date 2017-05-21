#include "Player.h"
#include "Gun.h"

#include <SDL/SDL.h>



Player::Player() : 
	m_currentGunIndex(-1)
{
}




Player::~Player()
{
}



void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets) {
	m_speed = speed;
	m_position = pos;
	m_inputManager = inputManager;
	m_bullets = bullets;
	m_camera = camera;
	m_color = Bengine::ColorRGBA8(0, 0, 185, 255);
	m_health = 150;
}





void Player::addGun(Gun* gun) {

	m_guns.push_back(gun);
	if (m_currentGunIndex == -1) {
		m_currentGunIndex = 0;
	}
}



void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime) {



	if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_UP)) {
		m_position.y += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_s) || m_inputManager->isKeyDown(SDLK_DOWN)) {
		m_position.y -= m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_LEFT)) {
		m_position.x -= m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_d) || m_inputManager->isKeyDown(SDLK_RIGHT)) {
		m_position.x += m_speed * deltaTime;
	}




	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		m_currentGunIndex = 0;
	}
	if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 0) {
		m_currentGunIndex = 1;
	}
	if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 0) {
		m_currentGunIndex = 2;
	}





	if (m_currentGunIndex != -1) {

		glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		m_guns[m_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			centerPosition,
			direction,
			// derefernce operator
			*m_bullets, 
			deltaTime);
	}


	collideWithLevel(levelData);

}

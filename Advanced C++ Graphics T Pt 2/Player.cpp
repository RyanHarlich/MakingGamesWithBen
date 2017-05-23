#include "Player.h"
#include "Gun.h"


/* NEW */
#include <Bengine/BengineErrors.h>


#include <SDL/SDL.h>



Player::Player() : 
	m_currentGunIndex(-1)
{
}




Player::~Player()
{
}


/* NEW: stance argumenmts */
void Player::init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances) {
	m_speed = speed;
	m_position = pos;
	m_inputManager = inputManager;
	m_bullets = bullets;
	m_camera = camera;
	m_health = 150;

	/* NEW: change color so solid white */
	//m_color = Bengine::ColorRGBA8(0, 0, 185, 255);
	m_color = Bengine::ColorRGBA8(255, 255, 255, 255);
	
	
	
	/* NEW */
	m_numStances = numStances;
	m_stanceIDs = stancesIDs;
	m_textureID = m_stanceIDs[m_currentMoveImage];
	/* NEW: end of new */


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


	Uint32 currentSpriteStance = IDLE;

	if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_UP)) {
		m_position.y += m_speed * deltaTime;

		/* NEW */
		currentSpriteStance = MOVING;

	}
	if (m_inputManager->isKeyDown(SDLK_s) || m_inputManager->isKeyDown(SDLK_DOWN)) {
		m_position.y -= m_speed * deltaTime;

		/* NEW */
		currentSpriteStance = MOVING;

	}
	if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_LEFT)) {
		m_position.x -= m_speed * deltaTime;

		/* NEW */
		currentSpriteStance = MOVING;

	}
	if (m_inputManager->isKeyDown(SDLK_d) || m_inputManager->isKeyDown(SDLK_RIGHT)) {
		m_position.x += m_speed * deltaTime;

		/* NEW */
		currentSpriteStance = MOVING;

	}




	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0) {
		m_currentGunIndex = 0;

		/* NEW */
		m_currentGun = PISTOL;

	}
	if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 0) {
		m_currentGunIndex = 1;

		/* NEW */
		m_currentGun = SHOTGUN;

	}
	if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 0) {
		m_currentGunIndex = 2;

		/* NEW */
		m_currentGun = RIFLE;

	}





	/* NEW */
	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);
	/* NEW: changed to private variable from agent.h */
	m_direction = glm::normalize(mouseCoords - centerPosition);


	if (m_currentGunIndex != -1) {

		/* NEW: removed and moved above */
		/*glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);
		glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);*/

		m_guns[m_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			centerPosition,

			/* NEW */
			//direction,
			m_direction,

			// derefernce operator
			*m_bullets, 
			deltaTime);



		/* NEW */
		if (m_inputManager->isKeyDown(SDL_BUTTON_LEFT)) {
			currentSpriteStance = SHOOTING;
		}
		/* NEW: end of new */

	}


	collideWithLevel(levelData);



	/* NEW */
	spriteStanceUpdate(currentSpriteStance | m_currentGun);

}



/* NEW */
void Player::spriteStanceUpdate(Uint32 currentSpriteStance) {

	// Change current move image start point for new position
	if ((currentSpriteStance != m_lastSpriteStance)) {
		if (currentSpriteStance == (IDLE | PISTOL)) {
			m_currentMoveImage = m_numStances.fPistolIdle - 1;
		}
		else if (currentSpriteStance == (IDLE | SHOTGUN)) {
			m_currentMoveImage = m_numStances.fShotgunIdle - 1;
		}
		else if (currentSpriteStance == (IDLE | RIFLE)) {
			m_currentMoveImage = m_numStances.fRifleIdle - 1;
		}
		else if (currentSpriteStance == (MOVING | PISTOL)) {
			m_currentMoveImage = m_numStances.fPistolMove - 1;
		}
		else if (currentSpriteStance == (MOVING | SHOTGUN)) {
			m_currentMoveImage = m_numStances.fShotgunMove - 1;
		}
		else if (currentSpriteStance == (MOVING | RIFLE)) {
			m_currentMoveImage = m_numStances.fRifleMove - 1;
		}
		else if (currentSpriteStance == (SHOOTING | PISTOL)) {
			m_currentMoveImage = m_numStances.fPistolShoot - 1;
		}
		else if (currentSpriteStance == (SHOOTING | SHOTGUN)) {
			m_currentMoveImage = m_numStances.fShotgunShoot - 1;
		}
		else if (currentSpriteStance == (SHOOTING | RIFLE)) {
			m_currentMoveImage = m_numStances.fRifleShoot - 1;
		}
	}


	m_lastSpriteStance = currentSpriteStance;


	// Image moving
	if (currentSpriteStance == (IDLE | PISTOL)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lPistolIdle) {
			m_currentMoveImage = m_numStances.fPistolIdle;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (IDLE | SHOTGUN)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lShotgunIdle) {
			m_currentMoveImage = m_numStances.fShotgunIdle;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (IDLE | RIFLE)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lRifleIdle) {
			m_currentMoveImage = m_numStances.fRifleIdle;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (MOVING | PISTOL)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lPistolMove) {
			m_currentMoveImage = m_numStances.fPistolMove;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (MOVING | SHOTGUN)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lShotgunMove) {
			m_currentMoveImage = m_numStances.fShotgunMove;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (MOVING | RIFLE)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lRifleMove) {
			m_currentMoveImage = m_numStances.fRifleMove;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (SHOOTING | PISTOL)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lPistolShoot) {
			m_currentMoveImage = m_numStances.fPistolShoot;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (SHOOTING | SHOTGUN)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lShotgunShoot) {
			m_currentMoveImage = m_numStances.fShotgunShoot;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
	else if (currentSpriteStance == (SHOOTING | RIFLE)) {
		++m_currentMoveImage;
		if (m_currentMoveImage > m_numStances.lRifleShoot) {
			m_currentMoveImage = m_numStances.fRifleShoot;
		}
		m_textureID = m_stanceIDs[m_currentMoveImage];
	}
}
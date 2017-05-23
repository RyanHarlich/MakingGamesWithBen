#pragma once

#include "Human.h"
#include "Bullet.h"

#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>


class Gun; 

class Player : public Human
{
public:
	Player();
	~Player();


	void init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, 
		float deltaTime) override;

private:

	void spriteStanceUpdate(Uint32 currentSpriteStance) override;

	Bengine::InputManager* m_inputManager;
	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
	Bengine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;

	Uint32 m_lastSpriteStance = IDLE | PISTOL;
	Uint32 m_currentGun = PISTOL;
};


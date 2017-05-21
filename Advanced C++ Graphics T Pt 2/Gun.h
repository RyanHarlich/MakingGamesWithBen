#pragma once
#include <string>
#include <vector>

#include "Bullet.h"

#include <glm/glm.hpp>

/* NEW */
#include <Bengine/AudioEngine.h>


class Gun
{
public:
	/* NEW: Bengine::SoundEffect fireEffect paramter */
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, int bulletDamage, float bulletSpeed, Bengine::SoundEffect fireEffect);
	~Gun();



	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

private:

	/* NEW */
	Bengine::SoundEffect m_fireEffect;

	std::string m_name;
	int m_fireRate; ///<< Fire rate in terms of frames
	int m_bulletsPerShot; ///< How many bullets are fired at a time
	float m_spread; ///<< Accuracy
	float m_bulletSpeed;
	int m_bulletDamage;
	float m_frameCounter;


};


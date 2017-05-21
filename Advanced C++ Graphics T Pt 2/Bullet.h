#pragma once
#include <vector>

#include <Bengine/SpriteBatch.h>

#include <glm/glm.hpp>



const int BULLET_RADIUS = 5;


class Human;
class Zombie;
class Agent;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();


	bool update(const std::vector<std::string>& levelData, float deltaTime);

	void draw(Bengine::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	// Getter
	float getDamage() const { return m_damage; }


private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_damage;
	float m_speed;
	Bengine::ColorRGBA8 m_color;
};


#pragma once

#include <Bengine/SpriteBatch.h>

#include <glm/glm.hpp>




const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;



class Zombie; 
class Human; 

class Agent
{
public:
	Agent();
	virtual ~Agent();


	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, float deltaTime) = 0;

	bool collideWithLevel(const std::vector<std::string>& levelData);

	bool collideWithAgent(Agent* agent);

	void draw(Bengine::SpriteBatch& spirteBatch);

	bool applyDamage(float damage);


	// Getter
	glm::vec2 getPosition() const { return _position; }

protected:
	
	void checkTilePosition(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& colideTilePosition,
		float x, 
		float y);

	void collideWithTile(glm::vec2 tilePos);


	glm::vec2 _position;	
	Bengine::ColorRGBA8 _color;
	float _speed;
	float _health;
};


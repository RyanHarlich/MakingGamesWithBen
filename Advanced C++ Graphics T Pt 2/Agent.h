#pragma once

#include <unordered_map>

#include "UploadStances.h"

#include <Bengine/SpriteBatch.h>




#include <glm/glm.hpp>
#include <SDL/SDL.h>


const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;





enum SpriteStance {
	IDLE = 0x1,
	MOVING = 0x2,
	SHOOTING = 0x4,
	PISTOL = 0x8,
	SHOTGUN = 0x16,
	RIFLE = 0x32
};



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
	glm::vec2 getPosition() const { return m_position; }


protected:
	
	void checkTilePosition(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& colideTilePosition,
		float x, 
		float y);

	void collideWithTile(glm::vec2 tilePos);

	virtual void spriteStanceUpdate(Uint32 currentSpriteStance = SpriteStance::MOVING) = 0;



	glm::vec2 m_position;	
	glm::vec2 m_direction = glm::vec2(1.0, 0.0f); // always starting facing right
	Bengine::ColorRGBA8 m_color;
	float m_speed;
	float m_health;
	glm::vec4 m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	GLuint m_textureID;
	std::unordered_map<unsigned int, GLuint> m_stanceIDs;
	NumStances m_numStances;
	unsigned int m_currentMoveImage = 0;





};


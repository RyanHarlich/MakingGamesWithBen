#pragma once

#include "Agent.h"

#include <glm/glm.hpp>



class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	/* NEW: added new stance arguements */
	void init(float speed, glm::vec2 pos, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, 
		float deltaTime) override;

private:

	/* NEW */
	virtual void spriteStanceUpdate(Uint32 currentSpriteStance = SpriteStance::MOVING) override;

	/* NEW: removed, and moved to agent.h */
	//glm::vec2 m_direction;


	int m_frames;
};


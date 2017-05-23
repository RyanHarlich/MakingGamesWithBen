#pragma once

#include "Agent.h"

#include <glm/glm.hpp>



class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies, 
		float deltaTime) override;

private:

	virtual void spriteStanceUpdate(Uint32 currentSpriteStance = SpriteStance::MOVING) override;



	int m_frames;
};


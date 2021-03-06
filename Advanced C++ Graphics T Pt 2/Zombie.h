#pragma once

#include "Agent.h"

#include <glm/glm.hpp>



class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 pos, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances);


	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

private:
	Human* getNearestHuman(std::vector<Human*>& humans);

	void spriteStanceUpdate(Uint32 currentSpriteStance = SpriteStance::MOVING) override;

	int m_frames;
	int m_framesZombieTransparency;
};


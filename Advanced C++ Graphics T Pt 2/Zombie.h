#pragma once

#include "Agent.h"

#include <glm/glm.hpp>



class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();

	void init(float speed, glm::vec2 pos);

	void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

private:
	Human* getNearestHuman(std::vector<Human*>& humans);
	glm::vec2 _direction;
	int _frames;
	int _framesZombieTransparency;
};


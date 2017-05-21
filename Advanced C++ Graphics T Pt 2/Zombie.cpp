#include <random>
#include <ctime>

#include "Zombie.h"
#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>



Zombie::Zombie() :
	m_frames(0),
	m_framesZombieTransparency(0)
{
}



Zombie::~Zombie()
{
}




void Zombie::init(float speed, glm::vec2 pos) {	
	m_speed = speed;
	m_position = pos;
	m_health = 150;
	m_color = Bengine::ColorRGBA8(0, 160, 0, 255);
}



void Zombie::update(const std::vector<std::string>& levelData, 
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime) {

	static std::mt19937 randomEngine(time(nullptr));

	++m_framesZombieTransparency;

	if (m_framesZombieTransparency == 3) {
		std::uniform_int_distribution<int> transparency(6, 220);
		m_color.a = transparency(randomEngine);
		m_framesZombieTransparency = 0;
	}



	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += direction * m_speed * deltaTime;
	}
	else {
		//TO DO: implement
	}



	if (collideWithLevel(levelData)) {
		std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));

	}

}





Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {

	Human* closestHuman = nullptr;

	float smallestDistance = 9999999.0f;


	for (unsigned int i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}


	return closestHuman;	
}

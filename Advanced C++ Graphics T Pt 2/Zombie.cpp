#include <random>
#include <ctime>

#include "Zombie.h"
#include "Human.h"

#include <glm/gtx/rotate_vector.hpp>



Zombie::Zombie() :
	_frames(0),
	_framesZombieTransparency(0)
{
}



Zombie::~Zombie()
{
}




void Zombie::init(float speed, glm::vec2 pos) {	
	_speed = speed;
	_position = pos;
	_health = 150;
	_color = Bengine::ColorRGBA8(0, 160, 0, 255);
}



void Zombie::update(const std::vector<std::string>& levelData, 
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime) {

	static std::mt19937 randomEngine(time(nullptr));

	++_framesZombieTransparency;

	if (_framesZombieTransparency == 3) {
		std::uniform_int_distribution<int> transparency(6, 220);
		_color.a = transparency(randomEngine);
		_framesZombieTransparency = 0;
	}



	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed * deltaTime;
	}
	else {
		//TO DO: implement
	}



	if (collideWithLevel(levelData)) {
		std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);
		_direction = glm::rotate(_direction, randRotate(randomEngine));

	}

}





Human* Zombie::getNearestHuman(std::vector<Human*>& humans) {

	Human* closestHuman = nullptr;

	float smallestDistance = 9999999.0f;


	for (unsigned int i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}


	return closestHuman;	
}

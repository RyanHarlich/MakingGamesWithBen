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


void Zombie::init(float speed, glm::vec2 pos, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances) {
	m_speed = speed;
	m_position = pos;
	m_health = 150;


	m_color = Bengine::ColorRGBA8(255, 255, 255, 255);



	m_numStances = numStances;
	m_stanceIDs = stancesIDs;
	m_textureID = m_stanceIDs[m_currentMoveImage];


}



void Zombie::update(const std::vector<std::string>& levelData, 
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime) {

	spriteStanceUpdate();


	static std::mt19937 randomEngine(time(nullptr));

	++m_framesZombieTransparency;

	if (m_framesZombieTransparency == 3) {
		std::uniform_int_distribution<int> transparency(6, 220);
		m_color.a = transparency(randomEngine);
		m_framesZombieTransparency = 0;
	}



	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += m_direction * m_speed * deltaTime;
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


	for (size_t i = 0; i < humans.size(); ++i) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}


	return closestHuman;	
}






void Zombie::spriteStanceUpdate(Uint32 currentSpriteStance /*  = SpriteStance::MOVING */) {

	++m_currentMoveImage;

	if (m_currentMoveImage > m_numStances.lMove) {
		m_currentMoveImage = m_numStances.fMove;
	}
	m_textureID = m_stanceIDs[m_currentMoveImage];
}

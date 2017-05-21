#include <random> 
#include <ctime>

#include "Human.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp> /* rotate() */



Human::Human() :
	m_frames(0)
{
}


Human::~Human()
{
}



void Human::init(float speed, glm::vec2 pos) {
	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0f); 

	m_health = 20;

	m_speed = speed;
	m_position = pos;


	m_color = Bengine::ColorRGBA8(200, 0, 200, 255);


	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);


	m_direction = glm::normalize(m_direction);

}




void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies, 
	float deltaTime) {

	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-40.0f, 40.0f);



	m_position += m_direction * m_speed * deltaTime;



	if (m_frames == 60) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		m_frames = 0;
	}
	else {
		++m_frames;
	}
	

	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}

}


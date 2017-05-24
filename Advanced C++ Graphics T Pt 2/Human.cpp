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


void Human::init(float speed, glm::vec2 pos, const std::unordered_map<unsigned int, GLuint>& stancesIDs, const NumStances& numStances) {


	static std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0f); 

	m_health = 20;

	m_speed = speed;
	m_position = pos;

	m_color = Bengine::ColorRGBA8(255, 255, 255, 255);
	
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));

	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);


	m_numStances = numStances;
	m_stanceIDs = stancesIDs;
	m_textureID = m_stanceIDs[m_currentMoveImage];

	m_uvRect = glm::vec4((float)m_currentMoveImage / 6.0f, 0.0f, 1.0f / 6.0f, 1.0f);

}




void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies, 
	float deltaTime) {


	spriteStanceUpdate();

	static std::mt19937 randomEngine((unsigned int)time(nullptr));
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





void Human::spriteStanceUpdate(Uint32 currentSpriteStance /*  = SpriteStance::MOVING */) {

	++m_currentMoveImage;

	if (m_currentMoveImage >= m_numStances.lMove) {
		m_currentMoveImage = m_numStances.fMove;
	}
	m_uvRect = glm::vec4(0.0f, (float)m_currentMoveImage / 6.0f, 1.0f, 1.0f / 6.0f);
}
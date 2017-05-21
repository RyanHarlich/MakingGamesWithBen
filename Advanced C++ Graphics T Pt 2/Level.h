#pragma once

#include <string>
#include <vector>


#include <Bengine/SpriteBatch.h>




const int TILE_WIDTH = 64;


class Level
{
public:
	
	Level(const std::string& fileName); 
	~Level();

	void draw();

	// Getters
	int getWidth() const { return m_levelData[0].size(); }
	int getHeight() const { return m_levelData.size(); }
	int getNumHumans() const { return m_numHumans; }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
	const glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2>& getZombieStartPositions() const { return m_zombieStartPositions; } 


private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	Bengine::SpriteBatch m_spriteBatch; 
	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_zombieStartPositions;
	std::vector<glm::vec2> m_humanStartPositions;

};


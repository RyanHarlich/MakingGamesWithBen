#include <fstream>

#include "Level.h"

#include <Bengine/ResourceManager.h>
#include <Bengine/BengineErrors.h>



Level::Level(const std::string& fileName)
{

	std::ifstream file; 
	file.open(fileName);

	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}


	std::string tmp;
	file >> tmp >> m_numHumans;
	std::getline(file, tmp);

	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}


	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	

	Bengine::ColorRGBA8 whiteColor(255, 255, 255, 255);

	
	for (size_t y = 0; y < m_levelData.size(); ++y) {
		for (size_t x = 0; x < m_levelData[y].size(); ++x) {

			
			char tile = m_levelData[y][x];

			
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			
			switch (tile) {
			case 'G': 
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/Wall/wall2_shga.png").id,
					0.0f,
					whiteColor);
				break;
			case 'R': 
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/Wall/wall1_color.png").id,
					0.0f,
					whiteColor);
				break;
			case 'P':
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/Wall/wall2_n.png").id,
					0.0f,
					whiteColor);
				break;
			case '@': 
				m_levelData[y][x] = '.'; 
				m_startPlayerPos.x = (float)x * TILE_WIDTH;
				m_startPlayerPos.y = (float)y * TILE_WIDTH;
				break;
			case 'Z': 
				m_levelData[y][x] = '.'; 
				m_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case 'H': 
				m_humanStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.': 
					break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)\n", tile, x, y);
				break;

				
			}
		}
	}

	m_spriteBatch.end();


}




Level::~Level()
{
}




void Level::draw() {
	m_spriteBatch.renderBatch();
}
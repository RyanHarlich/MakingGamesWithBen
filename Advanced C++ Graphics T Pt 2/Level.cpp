#include <fstream>

#include "Level.h"

#include <Bengine/ResourceManager.h>
#include <Bengine/Errors.h>



Level::Level(const std::string& fileName)
{

	std::ifstream file; 
	file.open(fileName);

	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}


	std::string tmp;
	file >> tmp >> _numHumans;
	std::getline(file, tmp);

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}


	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	

	Bengine::ColorRGBA8 whiteColor(255, 255, 255, 255);

	
	for (unsigned int y = 0; y < _levelData.size(); ++y) {
		for (unsigned int x = 0; x < _levelData[y].size(); ++x) {

			
			char tile = _levelData[y][x];

			
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			
			switch (tile) {
			case 'G': 
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/wezu_tex_cc_by/wall2_shga.png").id,
					0.0f,
					whiteColor);
				break;
			case 'R': 
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/wezu_tex_cc_by/wall1_color.png").id,
					0.0f,
					whiteColor);
				break;
			case 'P':
				_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/wezu_tex_cc_by/wall2_n.png").id,
					0.0f,
					whiteColor);
				break;
			case '@': 
				_levelData[y][x] = '.'; 
				_startPlayerPos.x = (float)x * TILE_WIDTH;
				_startPlayerPos.y = (float)y * TILE_WIDTH;
				break;
			case 'Z': 
				_levelData[y][x] = '.'; 
				_zombieStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case 'H': 
				_humanStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.': 
					break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)\n", tile, x, y);
				break;

				
			}
		}
	}

	_spriteBatch.end();


}




Level::~Level()
{
}




void Level::draw() {
	_spriteBatch.renderBatch();
}
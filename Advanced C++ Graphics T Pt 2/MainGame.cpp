#include <random>
#include <ctime>
#include <iostream>
#include <algorithm> /* min(,) */

#include "Gun.h"
#include "MainGame.h"
#include "Zombie.h"
#include "UploadStances.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include <Bengine/ResourceManager.h>

#include <glm/gtx/rotate_vector.hpp>


const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;



MainGame::MainGame() :
	m_screenWidth(1024),
	m_screenHeight(768),
	m_fps(0),
	m_player(nullptr),
	m_gameState(GameState::PLAY),
	m_numHumansKilled(0),
	m_numZombiesKilled(0)
{
}




MainGame::~MainGame()
{
	/* NEW: changed to size_t */
	/*for (unsigned int i = 0; i < m_levels.size(); ++i) {
		delete m_levels[i];
	}
	for (unsigned int i = 0; i < m_humans.size(); ++i) {
		delete m_humans[i];
	}
	for (unsigned int i = 0; i < m_zombies.size(); ++i) {
		delete m_zombies[i];
	}*/
	for (size_t i = 0; i < m_levels.size(); ++i) {
		delete m_levels[i];
	}
	for (size_t i = 0; i < m_humans.size(); ++i) {
		delete m_humans[i];
	}
	for (size_t i = 0; i < m_zombies.size(); ++i) {
		delete m_zombies[i];
	}
	delete m_spriteFont;
}



void MainGame::run() {

	initSystems();
	initLevel();


	Bengine::Music music = m_audioEngine.loadMusic("Sound/boss_0.ogg");
	music.play(-1);

	gameLoop();

}




void MainGame::initSystems() {

	Bengine::init();
	m_audioEngine.init();
	m_window.create("ZombieGame", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initShaders();

	m_agentSpriteBatch.init();

	m_hudSpriteBatch.init();
	m_spriteFont = new Bengine::SpriteFont("Fonts/lucon.ttf", 64);
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight /2));



	m_bloodPaticleBatch = new Bengine::ParticleBatch2D();

	m_bloodPaticleBatch->init(1000, 0.05f, Bengine::ResourceManager::getTexture("Textures/Blood/whitePuff06.png"), [](Bengine::Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.life * 255.0f);
	});

	m_particleEngine.addParticleBatch(m_bloodPaticleBatch);

}




void MainGame::initLevel() {

	m_levels.push_back(new Level("Levels/level1.txt"));


	m_currentLevel = 0;

	m_player = new Player();


	std::unordered_map<unsigned int, GLuint> playerStanceIDs;
	UploadStances playerStances;
	NumStances numStances = playerStances.uploadPlayerStances(playerStanceIDs);

	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets, playerStanceIDs, numStances);




	m_humans.push_back(m_player); 



	static std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));
	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2); 
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);




	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); ++i) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);



		std::unordered_map<unsigned int, GLuint> humanStanceIDs;
		UploadStances humanStances;
		NumStances numStances = humanStances.uploadHumanStances(humanStanceIDs);

		m_humans.back()->init(HUMAN_SPEED, pos,humanStanceIDs, numStances);
	}

	
	const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPositions();
	for (size_t i = 0; i < zombiePositions.size(); ++i) {
		m_zombies.push_back(new Zombie);

		

		std::unordered_map<unsigned int, GLuint> zombieStanceIDs;
		UploadStances zombieStances;
		NumStances numStances = zombieStances.uploadZombieStances(zombieStanceIDs);

		m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i], zombieStanceIDs, numStances);


	}




	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 0.06f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_Assault_Rifle_Single_Shot_01.wav")));
	m_player->addGun(new Gun("Shotgun", 30, 12, 0.7f, 4, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_Shotgun_Single_Shot.wav")));
	m_player->addGun(new Gun("MP5", 2, 1, 0.2f, 20, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/Futuristic_SMG_Single_Shot.wav")));

}


void MainGame::initShaders() {
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("fragmentPosition");
	m_textureProgram.addAttribute("fragmentColor");
	m_textureProgram.addAttribute("fragmentUV");
	m_textureProgram.linkShaders();

}


void MainGame::gameLoop() {

	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(20.0f);


	const float CAMERA_SCALE = 1.0f / 3.0f;


	m_camera.setScale(CAMERA_SCALE);


	const float MS_PER_SECOND = 1000; 
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; 
	const float MAX_DELTA_TIME = 1.0f; 

	/* NEW: changed to Uint32 */
	//float previousTicks = (float)SDL_GetTicks();
	Uint32 previousTicks = SDL_GetTicks();




	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();


		/* NEW: changed to Uint32 */
		//float newTicks = (float)SDL_GetTicks();
		Uint32 newTicks = SDL_GetTicks();


		/* NEW: changed to Uint32 */
		//float frameTime = newTicks - previousTicks;
		Uint32 frameTime = newTicks - previousTicks;

		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;


		checkVictory();

		m_inputManager.update();

		processInput();


		int i = 0;
		while (totalDeltaTime > 0.0f && i++ < MAX_PHYSICS_STEPS) {

			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			updateAgents(deltaTime);
			updateBullets(deltaTime);

			m_particleEngine.update(deltaTime);

			totalDeltaTime -= deltaTime;
		}





		m_camera.setPosition(m_player->getPosition());
		m_camera.update();

		m_hudCamera.update();

		drawGame();

		m_fps = fpsLimiter.end();


		std::cout << m_fps << std::endl;
	}
}




void MainGame::updateAgents(float deltaTime) {
	
	for (size_t i = 0; i < m_humans.size(); ++i) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}


	for (size_t i = 0; i < m_zombies.size(); ++i) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}


	for (size_t i = 0; i < m_zombies.size(); ++i) {	
		for (size_t j = i + 1; j < m_zombies.size(); ++j) { 
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}	

		for (size_t j = 1; j < m_humans.size(); ++j) { 	
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
		
				m_zombies.push_back(new Zombie);

				std::unordered_map<unsigned int, GLuint> zombieStanceIDs;
				UploadStances zombieStances;
				NumStances numStances = zombieStances.uploadZombieStances(zombieStanceIDs);

				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition(), zombieStanceIDs, numStances);

				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

	
		if (m_zombies[i]->collideWithAgent(m_player)) {
			std::cout << "YOU LOSE" << std::endl;
			std::cout << "Press any key to continue...\n";
			char tmp;
			std::cin >> tmp;
			m_gameState = GameState::EXIT;
		}
	}


	for (size_t i = 0; i < m_humans.size(); ++i) {	
		for (size_t j = i + 1; j < m_humans.size(); ++j) { 
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}





void MainGame::updateBullets(float deltaTime) {

	for (size_t i = 0; i < m_bullets.size();) {

		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else {
			++i;
		}
	}

	bool wasBulletRemoved;


	for (size_t i = 0; i < m_bullets.size(); ++i) {

		wasBulletRemoved = false;

		for (size_t j = 0; j < m_zombies.size();) {	
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {

				addBlood(m_bullets[i].getPosition(), 5);

				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {		
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					++m_numZombiesKilled;
				}
				else {
					++j;
				}

			
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				--i; 
		
				break;			
			}
			else {
				++j;
			}
		}




	
		if (wasBulletRemoved == false) {
			for (size_t j = 1; j < m_humans.size();) { 		
				if (m_bullets[i].collideWithAgent(m_humans[j])) {

					addBlood(m_bullets[i].getPosition(), 5);

					if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {

						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						++m_numHumansKilled;
					}
					else {
						++j;
					}

					
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					wasBulletRemoved = true;
					--i; 
					break;
				}
				else {
					++j;
				}
			}
		}
	}


}



void MainGame::checkVictory() {

	if (m_zombies.empty()) {
		printf("*** You win! ***\nYou killed %d humans and %d zombies. There are %d/%d civilians remaining\n",
			m_numHumansKilled,
			m_numZombiesKilled,
			m_humans.size() - 1, 
			m_levels[m_currentLevel]->getNumHumans());
		std::cout << "Press any key to continue...\n";
		char tmp;
		std::cin >> tmp;
		m_gameState = GameState::EXIT;
	}
}




void MainGame::processInput() {

	SDL_Event evnt;

	
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}






void MainGame::drawGame() {
	glClearDepth(1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0); 
	
	glUniform1i(m_textureProgram.getUniformLocation("mySampler"), 0); 

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]); 


	m_levels[m_currentLevel]->draw();


	m_agentSpriteBatch.begin();


	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	for (size_t i = 0; i < m_humans.size(); ++i) {
		if (m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}
	for (size_t i = 0; i < m_zombies.size(); ++i) {
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}
	for (size_t i = 0; i < m_bullets.size(); ++i) {
		m_bullets[i].draw(m_agentSpriteBatch);
	}
	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	m_particleEngine.draw(&m_agentSpriteBatch);

	drawHud();

	m_textureProgram.unuse();
	m_window.swapBuffer();
}





void MainGame::drawHud() {
	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num Humans %d", m_humans.size() - 1);


	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0), 
		glm::vec2(0.5), 0.0f, Bengine::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36),
		glm::vec2(0.5), 0.0f, Bengine::ColorRGBA8(255, 255, 255, 255));

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}






void MainGame::addBlood(const glm::vec2& position, int numParticles) {

	static std::mt19937 randEngine(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 vel(2.0f, 0.0f);
	
	for (int i = 0; i < numParticles; ++i) {
		m_bloodPaticleBatch->addParticle(position, glm::rotate(vel, randAngle(randEngine)), Bengine::ColorRGBA8(255, 0, 0, 255), 30.0f);
	}
}
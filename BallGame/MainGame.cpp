#define _CRT_SECURE_NO_WARNINGS // NEW To shut up the compiler about sprintf...
#include "MainGame.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm> /* min(,) */

#include <Bengine/Bengine.h>
#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>


// Some helpful constant.
const float DESIRE_FPS = 60.0f; // FPS that game is designed to run at
const int MAX_PHYSICS_STEPS = 6; // Max number of physcis step per frame
const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRE_FPS; // The desired frame time per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime





void MainGame::run() {

	init();
	initBalls();

	// Start our previousTicks variable
	Uint32 previousTicks = SDL_GetTicks();

	// Game loop
	while (m_gameState == GameState::RUNNING) {
		m_fpsLimiter.begin();
		processInput();

		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

		int i = 0;

		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {

			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			update(deltaTime);

			totalDeltaTime -= deltaTime;
			++i;
		}

		m_camera.update();
		draw();
		m_fps = m_fpsLimiter.end();
	}
}




void MainGame::init() {

	Bengine::init();
	m_audioEngine.init();
	Bengine::Music music = m_audioEngine.loadMusic("Audio/Music/Ice-Shine-Bells_0.ogg");
	music.play();

	// This size was bigger than full screen
	/*m_screenWidth = 1920;
	m_screenHeight = 1080;*/
	m_screenWidth = 1476;
	m_screenHeight = 900;

	m_window.create("BallGame", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_camera.init(m_screenWidth, m_screenHeight);
	// Point the camera to the center of the screen
	m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));

	m_spriteBatch.init();
	m_spriteFont = std::make_unique<Bengine::SpriteFont>("Fonts/lucon.ttf", 40);

	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
	
	m_fpsLimiter.setMaxFPS(DESIRE_FPS);
}






struct BallSpawn {
	BallSpawn(const Bengine::ColorRGBA8& colr,
		float rad, float m, float minSpeed,
		float maxSpeed, float prob):
		color(colr),
		radius(rad),
		mass(m),
		randSpeed(minSpeed, maxSpeed),
		probability(prob) { // probability that the ball spawns
	}
	Bengine::ColorRGBA8 color;
	float radius;
	float mass;
	float probability;
	std::uniform_real_distribution<float> randSpeed;
};







void MainGame::initBalls() {

	/* NEW: initialize the grid */
	m_grid = std::make_unique<Grid>(m_screenWidth, m_screenHeight, CELL_SIZE);



#define ADD_BALL(p, ...) \
	totalProbability += p; \
	possibleBalls.emplace_back(__VA_ARGS__);


	std::mt19937 randomEngine((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> randX(0.0f, (float)m_screenWidth);
	std::uniform_real_distribution<float> randY(0.0f, (float)m_screenHeight);
	std::uniform_real_distribution<float> randDir(-1.0f, 1.0);

	// Add all possible balls
	std::vector<BallSpawn> possibleBalls;
	float totalProbability = 0.0f;

	
	const int NUM_BALLS = 100;
	ADD_BALL(20.0f, Bengine::ColorRGBA8(255, 255, 255, 255),
		20.0f, 1.0f, 0.1f, 7.0f, totalProbability);
	ADD_BALL(10.0f, Bengine::ColorRGBA8(0, 0, 255, 255),
		30.0f, 2.0f, 0.1f, 3.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::ColorRGBA8(255, 0, 0, 255),
		50.0f, 4.0f, 0.0f, 0.0f, totalProbability);


	// Random probability for ball spawn
	std::uniform_real_distribution<float> spawn(0.0f, totalProbability);

	// Small optimization that sets the size of the internal array to prevent
	// extra allocations
	/* NEW: nothing was changed! But pointing out that if a pointer to a ball in a vector was pointing to one, and the vector changed, the pointer would lose the ball and no longer be pointing to that ball because as a vector changes it shifts around in memory and changes, this following line will prevent that from happening because the vector has been sized and would not be resized later unless done so, so its spot in memory will not change */
	m_balls.reserve(NUM_BALLS);

	BallSpawn* ballToSpawn = &possibleBalls[0];
	for (int i = 0; i < NUM_BALLS; ++i) {
		// Get the ball spawn rool
		float spawnVal = spawn(randomEngine);

		for (size_t i = 0; i < possibleBalls.size(); ++i) {
			if (spawnVal <= possibleBalls[i].probability) {
				ballToSpawn = &possibleBalls[i];
				break;
			}
		}

		// Get random starting posistion
		glm::vec2 pos(randX(randomEngine), randY(randomEngine));

		// Hacky way to get a random direction
		glm::vec2 direction(randDir(randomEngine), randDir(randomEngine));
		if (direction.x != 0.0f || direction.y != 0.0f) {
			direction = glm::normalize(direction);
		}
		else {
			direction = glm::vec2(1.0f, 0.0f); // default direction
		}

		// Add ball
		m_balls.emplace_back(ballToSpawn->radius, ballToSpawn->mass, pos, direction * ballToSpawn->randSpeed(randomEngine), Bengine::ResourceManager::getTexture("Texture/CircleSH.png").id, ballToSpawn->color);


		/* NEW: add the ball to the grid. If you ever call emplace back after init balls, m_grid will have dangling pointers! */
		m_grid->addBall(&m_balls.back());

	}

}







void MainGame::update(float deltaTime) {
	m_ballController.updateBalls(m_balls, deltaTime, m_screenWidth, m_screenHeight);
}





void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);

	glUniform1i(m_textureProgram.getUniformLocation("mySampler"), 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);





	m_spriteBatch.begin();


	for (auto& ball : m_balls) {
		m_ballRenderer.renderBall(m_spriteBatch, ball);
	}


	m_spriteBatch.end();
	m_spriteBatch.renderBatch();


	drawHud();

	m_textureProgram.unuse();
	m_window.swapBuffer();
}












void MainGame::drawHud() {
	const Bengine::ColorRGBA8 fontColor(255, 0, 0, 255);
	char buffer[64];
	sprintf(buffer, "%.1f", m_fps);

	m_spriteBatch.begin();
	m_spriteFont->draw(m_spriteBatch, buffer, glm::vec2(0.0f, m_screenHeight - 50.0f),
		glm::vec2(1.0), 0.0f, fontColor);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
}




void MainGame::processInput() {

	SDL_Event evnt;


	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_ballController.onMouseMove(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
			m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_ballController.onMouseDown(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_ballController.onMouseUp(m_balls);
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}

	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_gameState = GameState::EXIT;
	}
	// Handle gravity changes
	if (m_inputManager.isKeyPressed(SDLK_LEFT)) {
		m_ballController.setGravityDirection(GravityDirection::LEFT);
	} else if (m_inputManager.isKeyPressed(SDLK_RIGHT)) {
		m_ballController.setGravityDirection(GravityDirection::RIGHT);
	}
	else if (m_inputManager.isKeyPressed(SDLK_UP)) {
		m_ballController.setGravityDirection(GravityDirection::UP);
	}
	else if (m_inputManager.isKeyPressed(SDLK_DOWN)) {
		m_ballController.setGravityDirection(GravityDirection::DOWN);
	}
	else if (m_inputManager.isKeyPressed(SDLK_SPACE)) {
		m_ballController.setGravityDirection(GravityDirection::NONE);
	}



}
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


/* NEW */
MainGame::~MainGame() {
	for (size_t i = 0; i < m_ballRenderers.size(); ++i) {
		delete m_ballRenderers[i];
	}
}



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
	/* NEW: screen size was changed from 2560x1440 to the 1920x1080 */
	/* NEW: IMPORTANT: the fps problem seemed to have been from prior to the adding of more varietys of balls */
	/*m_screenWidth = 2560;
	m_screenHeight = 1440;*/
	m_screenWidth = 1920;   // this one matches the tutorial for the amount of collision so the fps will be better (with the updated collision copied from GitHub and noted in a /* NEW: __ */
	m_screenHeight = 1080;
	/* NEW: so the update for this particular computer looks like this in ~ratio (%75) */
	//m_screenWidth = 1476; // switched back because this was 60 fps
	//m_screenHeight = 900;
	/*m_screenWidth = 1107; // to much collision
	m_screenHeight = 675;*/

	m_window.create("Ball Game", m_screenWidth, m_screenHeight, 0);
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


	/* NEW */
	initRenderers();
}






/* NEW */
void MainGame::initRenderers() {
	m_ballRenderers.push_back(new BallRenderer);
	m_ballRenderers.push_back(new MomentumBallRenderer);
	m_ballRenderers.push_back(new VelocityBallRenderer(m_screenWidth, m_screenHeight));
	m_ballRenderers.push_back(new TrippyBallRenderer(m_screenWidth, m_screenHeight));
	m_ballRenderers.push_back(new SelfMade);
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


	/* NEW: for random balls */
	/// Random values for ball types
	std::uniform_real_distribution<float> r1(2.0f, 6.0f);
	std::uniform_int_distribution<int> r2(0, 255);


	/* NEW: changed from 40,000 to 20,000 */
	const int NUM_BALLS = 20000;
	/* NEW: different variaty of balls */
	ADD_BALL(1.0f, Bengine::ColorRGBA8(255, 255, 255, 255),
		2.0f, 1.0f, 0.1f, 7.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::ColorRGBA8(1, 254, 145, 255),
		2.0f, 2.0f, 0.1f, 3.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::ColorRGBA8(177, 0, 254, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability)
	ADD_BALL(1.0f, Bengine::ColorRGBA8(254, 0, 0, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::ColorRGBA8(0, 255, 255, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	ADD_BALL(1.0f, Bengine::ColorRGBA8(255, 255, 0, 255),
		3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	///* NEW: adds a bunch of different balls adding random values */
	//// Make a bunch of random ball types
	for (int i = 0; i < 10000; ++i) {
		ADD_BALL(1.0f, Bengine::ColorRGBA8(r2(randomEngine), r2(randomEngine), r2(randomEngine), 255), r1(randomEngine), r1(randomEngine), 0.0f, 0.0f, totalProbability);
	}
	//ADD_BALL(2.0f, Bengine::ColorRGBA8(255, 255, 255, 255),
	//	1.0f, 1.0f, 0.1f, 7.0f, totalProbability);
	//ADD_BALL(10.0f, Bengine::ColorRGBA8(0, 0, 255, 255),
	//	2.0f, 2.0f, 0.1f, 3.0f, totalProbability);
	//ADD_BALL(1.0f, Bengine::ColorRGBA8(255, 0, 0, 255),
	//	3.0f, 4.0f, 0.0f, 0.0f, totalProbability);
	


	// Random probability for ball spawn
	std::uniform_real_distribution<float> spawn(0.0f, totalProbability);

	// Small optimization that sets the size of the internal array to prevent
	// extra allocations
	m_balls.reserve(NUM_BALLS);

	BallSpawn* ballToSpawn = &possibleBalls[0];
	for (int i = 0; i < NUM_BALLS; ++i) {
		// Get the ball spawn rool
		float spawnVal = spawn(randomEngine);

		for (size_t j = 0; j < possibleBalls.size(); ++j) {
			if (spawnVal <= possibleBalls[j].probability) {
				ballToSpawn = &possibleBalls[j];
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

		/* NEW: velocity changed to not be reduced by half, which did not have in the first place */
		// Add ball
		/*m_balls.emplace_back(ballToSpawn->radius, ballToSpawn->mass, pos, direction * ballToSpawn->randSpeed(randomEngine) * 0.5f, Bengine::ResourceManager::getTexture("Texture/CircleSH.png").id, ballToSpawn->color); */
		m_balls.emplace_back(ballToSpawn->radius, ballToSpawn->mass, pos, direction * ballToSpawn->randSpeed(randomEngine), /* NEW: change was suppose to be here */
			Bengine::ResourceManager::getTexture("Texture/CircleSH.png").id, ballToSpawn->color);


		m_grid->addBall(&m_balls.back());

	}

}







void MainGame::update(float deltaTime) {
	m_ballController.updateBalls(m_balls, m_grid.get(), deltaTime, m_screenWidth, m_screenHeight);
}





void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* NEW: moved so only the heads up display is using this textureProgram */
	//m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	/* NEW: moved to BallRenderer class, but the heads up display does need this down below */
	//glUniform1i(m_textureProgram.getUniformLocation("mySampler"), 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

	/* NEW: moved to BallRenderer class, but the heads up display does need this down below */
	//glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);




	/* NEW: since the Lazy int unuses the shader program in BallRender, the program will not work unless renderingBatch is called before the shader program is unused, so move all spriteBatch things (omit init) to the BallRenderer.cpp */
	//m_spriteBatch.begin();

	/* NEW: loop is in the BallRenderer class*/
	/*for (auto& ball : m_balls) {
		m_ballRenderer.renderBall(m_spriteBatch, ball);
	}*/
	/* NEW: NEW: renderer selection */
	//m_ballRenderer.renderBall(m_spriteBatch, m_balls, projectionMatrix);
	m_ballRenderers[m_currentRenderer]->renderBalls(m_spriteBatch, m_balls, projectionMatrix);




	/* NEW: since the Lazy int unuses the shader program in BallRender, the program will not work unless renderingBatch is called before the shader program is unused */
	/*m_spriteBatch.end();
	m_spriteBatch.renderBatch();*/


	/* NEW: moved so only the heads up display is using this textureProgram */
	m_textureProgram.use();
	/* NEW: moved here as well as the BallRenderer class, because the heads up display does need this */
	glUniform1i(m_textureProgram.getUniformLocation("mySampler"), 0);
	/* NEW: moved here as well as the BallRenderer class, because the heads up display does need this */
	glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);

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

	/* NEW: update inuput manager so that can tell if key is pressed for switching between renderers which was never called yet, for some reason it was working fine before */
	m_inputManager.update();

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




	/* NEW: changed to a new renderer */
	if (m_inputManager.isKeyPressed(SDLK_1)) {
		++m_currentRenderer;
		if (m_currentRenderer >= (int)m_ballRenderers.size()) {
			m_currentRenderer = 0;
		}
	}



}
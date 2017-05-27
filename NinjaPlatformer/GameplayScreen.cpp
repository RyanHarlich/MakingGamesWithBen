/* NEW */
#include <random>
#include <ctime>


#include "GameplayScreen.h"

#include <Bengine/IMainGame.h>

/* NEW */
#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>




/* NEW: paramter and initialization list */
GameplayScreen::GameplayScreen(Bengine::Window* window) : m_window(window)
{
}


GameplayScreen::~GameplayScreen()
{
}


int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}


int GameplayScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}


void GameplayScreen::build() {
}



void GameplayScreen::destroy() {
}



void GameplayScreen::onEntry() {
	/* NEW: gravity of earth ms^2*/
	b2Vec2 gravity(0.0f, -9.81f); 
	m_world = std::make_unique<b2World>(gravity);

	// Make the ground
	b2BodyDef groundBodyDef; // body definition for the specific body
	groundBodyDef.position.Set(0.0f, -22.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef); // pass in body definition to the world to create an actual body
	// Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f); // sets fixture as a box, half x half y, so actually 100 meters wide by 20 meters tall
	groundBody->CreateFixture(&groundBox, 0.0f); // create the fixture of the groundBody with the newly created groundBox definition

	// Make a bunch of boxes
	std::mt19937 randGenerator((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);
	const int NUM_BOXES = 50;
	

	for (int i = 0; i < NUM_BOXES; ++i) {	
		Box newBox;
		float s = size(randGenerator);

		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(s, s), Bengine::ColorRGBA8(color(randGenerator), color(randGenerator), color(randGenerator), 255));
		m_boxes.push_back(newBox);
	}


	m_spriteBatch.init();
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
	m_texture = Bengine::ResourceManager::getTexture("Textures/Box.png");
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0); // 32 pixels per meter
	/* NEW: end of new */
}



void GameplayScreen::onExit() {
}



void GameplayScreen::update() {

	/* NEW */
	m_camera.update();

	checkInput();

	/* NEW */
	// update the physics simulation
	m_world->Step(1.0f / 60.0f, 6, 2); // Step: 1 over the frame rate, Velocity Iterations: recommend 6, Position Iterations: recommend 2 (higher this is the slower it will run the more competition it will use but the more precise it will be, the lower it is the less precise it will be)
}



void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* NEW */
	m_textureProgram.use();

	// Upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// Camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);


	m_spriteBatch.begin();

	// Draw all the boxes
	for (auto& b : m_boxes) {
		glm::vec4 destRect;

		// The position of the box is actually the center of the box not the corner of the box
		destRect.x = b.getBody()->GetPosition().x - (b.getDimensions().x / 2.0f);
		destRect.y = b.getBody()->GetPosition().y - (b.getDimensions().y / 2.0f);

		destRect.z = b.getDimensions().x;
		destRect.w = b.getDimensions().y;
		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_texture.id, 0.0f, b.getColor(), b.getBody()->GetAngle());

	}

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();
	/* NEW: end of new */
}



void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}



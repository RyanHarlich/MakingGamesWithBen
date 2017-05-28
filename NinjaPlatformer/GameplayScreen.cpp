#include <random>
#include <ctime>

#include "GameplayScreen.h"

#include <Bengine/IMainGame.h>
#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>




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

	/* NEW: gravity was to low, so increased to a guessed number */
	//b2Vec2 gravity(0.0f, -9.81f); // gravity of earth m/s^2
	b2Vec2 gravity(0.0f, -25.00f); // gravity of earth m/s^2



	m_world = std::make_unique<b2World>(gravity);

	// Make the ground
	b2BodyDef groundBodyDef; // body definition for the specific body
	groundBodyDef.position.Set(0.0f, -22.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef); // pass in body definition to the world to create an actual body
	// Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f); // sets fixture as a box, half x half y, so actually 100 meters wide by 20 meters tall
	groundBody->CreateFixture(&groundBox, 0.0f); // create the fixture of the groundBody with the newly created groundBox definition


	/* NEW: moved above before creating the boxes */
	m_texture = Bengine::ResourceManager::getTexture("Textures/Box.png");

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

		/* NEW: added a fixed rotation argument */
		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(s, s), m_texture,Bengine::ColorRGBA8(color(randGenerator), color(randGenerator), color(randGenerator), 255), false);

		m_boxes.push_back(newBox);
	}


	m_spriteBatch.init();
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	/* NEW: moved above before creating the boxes */
	//m_texture = Bengine::ResourceManager::getTexture("Textures/Box.png");

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0); // 32 pixels per meter


	/* NEW */
	// Init player
	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(1.0f, 2.0f), Bengine::ColorRGBA8(255,255,255,255), true);


}



void GameplayScreen::onExit() {
}



void GameplayScreen::update() {

	m_camera.update();

	checkInput();

	/* NEW */
	m_player.update(m_game->inputManager);

	// update the physics simulation
	m_world->Step(1.0f / 60.0f, 6, 2); // Step: 1 over the frame rate, Velocity Iterations: recommend 6, Position Iterations: recommend 2 (higher this is the slower it will run the more competition it will use but the more precise it will be, the lower it is the less precise it will be)
}



void GameplayScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


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
		/* NEW */
		b.draw(m_spriteBatch);

		/* NEW: removed and moved to new Box draw function */
		//glm::vec4 destRect;

		//// The position of the box is actually the center of the box not the corner of the box
		//destRect.x = b.getBody()->GetPosition().x - (b.getDimensions().x / 2.0f);
		//destRect.y = b.getBody()->GetPosition().y - (b.getDimensions().y / 2.0f);

		//destRect.z = b.getDimensions().x;
		//destRect.w = b.getDimensions().y;
		//m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_texture.id, 0.0f, b.getColor(), b.getBody()->GetAngle());
	}
	
	/* NEW */
	// Draw the player
	m_player.draw(m_spriteBatch);



	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();

}



void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}
}



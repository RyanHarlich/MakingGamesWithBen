#include <random>
#include <ctime>

#include "GameplayScreen.h"
#include "Light.h"

/* NEW */
#include "ScreenIndices.h"

#include <Bengine/IMainGame.h>
#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>



GameplayScreen::GameplayScreen(Bengine::Window* window) : m_window(window)
	/* NEW */
	//*** If protected member is defined in base class cannot use it in the initialization list in the constructor of the subclass, instead have to set it equal to in the constructors block statement ***
	//m_screenIndex(Screen_INDEX_GAMEPLAY)
{
		/* NEW */
		m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}


GameplayScreen::~GameplayScreen()
{
}


int GameplayScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}


int GameplayScreen::getPreviousScreenIndex() const {

	/* NEW: previous screen enabled */
	//return SCREEN_INDEX_NO_SCREEN;
	return SCREEN_INDEX_MAINMENU;
}


void GameplayScreen::build() {
}



void GameplayScreen::destroy() {
}



void GameplayScreen::onEntry() {

	//b2Vec2 gravity(0.0f, -9.81f); // gravity of earth m/s^2
	b2Vec2 gravity(0.0f, -25.00f); // gravity of earth m/s^2



	m_world = std::make_unique<b2World>(gravity);



	m_debugRenderer.init();



	// Make the ground
	b2BodyDef groundBodyDef; // body definition for the specific body
	groundBodyDef.position.Set(0.0f, -22.0f);
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef); // pass in body definition to the world to create an actual body
	// Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f); // sets fixture as a box, half x half y, so actually 100 meters wide by 20 meters tall
	groundBody->CreateFixture(&groundBox, 0.0f); // create the fixture of the groundBody with the newly created groundBox definition


	m_texture = Bengine::ResourceManager::getTexture("Textures/Box.png");

	// Make a bunch of boxes
	std::mt19937 randGenerator((unsigned int)time(nullptr));
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-10.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);
	std::uniform_int_distribution<int> color(0, 255);


	const int NUM_BOXES = 10;


	for (int i = 0; i < NUM_BOXES; ++i) {	
		Box newBox;

		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), m_texture,Bengine::ColorRGBA8(color(randGenerator), color(randGenerator), color(randGenerator), 255), false);

		m_boxes.push_back(newBox);
	}


	m_spriteBatch.init();
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();




	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();




	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0); // 32 pixels per meter


	// Init player
	m_player.init(m_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), Bengine::ColorRGBA8(255,255,255,255), true);




	/* NEW */
	initUI();

	/* NEW: removed and moved to new method initUI() */
	/*
		// Init the UI, call this after initializing all the openGL stuff and glew 
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("AlfiskoSkin.scheme");
	m_gui.setFont("DejaVuSans-10"); // do not include the .font for font
									// Change between the AlfiskoSkin and the TaharezLook by changing the part in front of ".../Button"
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton")); // 50%x 50%y is in the middle, then the dimensions follow
	testButton->setText("Hello World!");
	CEGUI::Combobox* testCombobox = static_cast<CEGUI::Combobox*>(m_gui.createWidget("TaharezLook/Combobox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "TestCombobox"));
	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	// need to injectMousePosition to show, which is done in GUI.cpp onSDLEvent which allows for GUIs to become interactive
	m_gui.showMouseCursor();
	// disables the default mouse cursor so only see the CEGUI mouse cursor, 1 means to show the cursor and 0 means to not show the cursor
	SDL_ShowCursor(0);
	*/
}



void GameplayScreen::onExit() {
	m_debugRenderer.dispose();
}



void GameplayScreen::update() {

	m_camera.update();

	checkInput();


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
		b.draw(m_spriteBatch);
	}
	

	// Draw the player
	m_player.draw(m_spriteBatch);



	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	m_textureProgram.unuse();



	// Debug renderer
	if (m_renderDebug) {
		glm::vec4 destRect;
		for (auto& b : m_boxes) {
			destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
			destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
			destRect.z = b.getDimensions().x;
			destRect.w = b.getDimensions().y;
			m_debugRenderer.drawBox(destRect, Bengine::ColorRGBA8(255,255,255,255), b.getBody()->GetAngle());
		}
		// Render player
		m_player.drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}




	// Render some test lights
	// TODO: Do not hard code this
	Light playerLight;
	playerLight.color = Bengine::ColorRGBA8(255, 255, 255, 128);
	playerLight.position = m_player.getPosition();
	playerLight.size = 30.0f;

	// Should use a vector for all the lights, but this is for showing and is hacky
	Light mouseLight;
	mouseLight.color = Bengine::ColorRGBA8(255, 0, 255, 128);
	glm::vec2 mousePos = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
	mouseLight.position = mousePos;
	mouseLight.size = 45.0f;


	m_lightProgram.use();
	glUniformMatrix4fv(m_textureProgram.getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);

	// Additive blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();
	playerLight.draw(m_spriteBatch);
	mouseLight.draw(m_spriteBatch);
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_lightProgram.unuse();

	// reset to regular alpha blending 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	// want to draw widget last
	m_gui.draw();

}





/* NEW */
void GameplayScreen::initUI() {

	// Init the UI, call this after initializing all the openGL stuff and glew 
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("AlfiskoSkin.scheme");
	m_gui.setFont("DejaVuSans-10"); // do not include the .font for font
									// Change between the AlfiskoSkin and the TaharezLook by changing the part in front of ".../Button"
	CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.5f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton")); // 50%x 50%y is in the middle, then the dimensions follow




	/* NEW: changed text */
	//testButton->setText("Hello World!");
	testButton->setText("Exit Game!");

	/* NEW */
	// although this is in the init function, it subscribes the event permanently
	// Set the event to be called when we click, event and subscriber which is the function to point at and object to point at such as this for current object
	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));




	CEGUI::Combobox* testCombobox = static_cast<CEGUI::Combobox*>(m_gui.createWidget("TaharezLook/Combobox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "TestCombobox"));
	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	// need to injectMousePosition to show, which is done in GUI.cpp onSDLEvent which allows for GUIs to become interactive
	m_gui.showMouseCursor();
	// disables the default mouse cursor so only see the CEGUI mouse cursor, 1 means to show the cursor and 0 means to not show the cursor
	SDL_ShowCursor(0);

}







void GameplayScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);

		m_gui.onSDLEvent(evnt);

	}
}







/* NEW: give prefix 'on' for event handling  */
// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
	m_currentState = Bengine::ScreenState::EXIT_APPLICATION;
	return true;
}




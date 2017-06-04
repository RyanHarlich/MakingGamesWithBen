#include "EditorScreen.h"
#include "ScreenIndices.h"

#include <Bengine/ResourceManager.h>


EditorScreen::EditorScreen(Bengine::Window* window) :
	m_window(window),
	m_spriteFont("Fonts/lucon.ttf", 32)
{
	m_screenIndex = SCREEN_INDEX_EDITOR;
}


EditorScreen::~EditorScreen() {
}

int EditorScreen::getNextScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

int EditorScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_MAINMENU;
}

void EditorScreen::build() {
}

void EditorScreen::destroy() {
}

void EditorScreen::onEntry() {

	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(1.0f); // 1 pixel, so each pixel is only 1 pixel

	initUI();
	m_spriteBatch.init();
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_blankTexture = Bengine::ResourceManager::getTexture("Textures/Blank.png");
}

void EditorScreen::onExit() {
	m_gui.destroy();
	m_textureProgram.dispose();
}

void EditorScreen::update() {
	m_camera.update();
	checkInput();
}

void EditorScreen::draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Dark blue background

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

	// Scope this to make it a mini procedure where variables can not be used outside of the block statement scope, so can declare destRec multiple times if need to in this method
	{ // Draw the color picker quad	

		const float QUAD_SIZE = 75.0f;
			
		glm::vec4 destRect;
		destRect.x = m_bSlider->getXPosition().d_scale * m_window->getScreenWidth() + 20.0f - m_window->getScreenWidth() / 2.0f + QUAD_SIZE / 2.0f;
		destRect.y = m_window->getScreenHeight() / 2.0f - m_bSlider->getYPosition().d_scale * m_window->getScreenHeight() - m_bSlider->getHeight().d_scale * m_window->getScreenHeight() * 0.5f - QUAD_SIZE / 2.0f;
		destRect.z = QUAD_SIZE; // width
		destRect.w = QUAD_SIZE; // height

		m_spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_blankTexture.id, 0.0f, Bengine::ColorRGBA8((GLubyte)m_colorPickerRed, (GLubyte)m_colorPickerGreen, (GLubyte)m_colorPickerBlue, 255));
	}

	{ // Draw labels for rigid and dynamic buttons

		glm::vec2 pos;
		pos.x = m_rigidRadioButton->getXPosition().d_scale * m_window->getScreenWidth() - m_window->getScreenWidth() / 2.0f + m_rigidRadioButton->getWidth().d_offset / 2.0f; // d_offset uses the destRectPixels instead of the destRectPercentage like d_scale
		pos.y = m_window->getScreenHeight() / 2.0f - m_rigidRadioButton->getYPosition().d_scale * m_window->getScreenHeight();
		m_spriteFont.draw(m_spriteBatch, "Rigid", pos, glm::vec2(0.46f), 0.0f, Bengine::ColorRGBA8(255,255,255,255), Bengine::Justification::MIDDLE);

		pos.x = m_dynamicRadioButton->getXPosition().d_scale * m_window->getScreenWidth() - m_window->getScreenWidth() / 2.0f + m_dynamicRadioButton->getHeight().d_offset / 2.0f; // d_offset uses the destRectPixels instead of the destRectPercentage like d_scale
		pos.y = m_window->getScreenHeight() / 2.0f - m_dynamicRadioButton->getYPosition().d_scale * m_window->getScreenHeight();
		m_spriteFont.draw(m_spriteBatch, "Dynamic", pos, glm::vec2(0.46f), 0.0f, Bengine::ColorRGBA8(255, 255, 255, 255), Bengine::Justification::MIDDLE);

	}
	
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();
	m_gui.draw();
}

void EditorScreen::initUI() {
	// Init the UI, call this after initializing all the openGL stuff and glew 
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.setFont("DejaVuSans-10");


	// Scope this to make it a mini procedure where variables can not be used outside of the block statement scope
	{ // Add the color picker
		const float X_DIM = 0.015f, Y_DIM = 0.1f;
		const float Y_POS = 0.05f;
		const float PADDING = 0.005f;

		// Create a red slider
		m_rSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.05f, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "RedSlider"));
		m_rSlider->setMaxValue(255.0f);
		m_rSlider->setCurrentValue(m_colorPickerRed); // sets the value of the slider to the following
		m_rSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerRedChanged, this));
		m_rSlider->setClickStep(1.0f); // makes increments by one and not by .01

		// Create a green slider
		m_gSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.05f + X_DIM + PADDING, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "GreenSlider"));
		m_gSlider->setMaxValue(255.0f);
		m_gSlider->setCurrentValue(m_colorPickerGreen); // sets the value of the slider to the following
		m_gSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerGreenChanged, this));
		m_gSlider->setClickStep(1.0f); // makes increments by one and not by .01

		// Create a blue slider
		m_bSlider = static_cast<CEGUI::Slider*>(m_gui.createWidget("TaharezLook/Slider", glm::vec4(0.05f + (X_DIM + PADDING) * 2, Y_POS, X_DIM, Y_DIM), glm::vec4(0.0f), "BlueSlider"));
		m_bSlider->setMaxValue(255.0f);
		m_bSlider->setCurrentValue(m_colorPickerBlue); // sets the value of the slider to the following
		m_bSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&EditorScreen::onColorPickerBlueChanged, this));
		m_bSlider->setClickStep(1.0f); // makes increments by one and not by .01
	}


	{ // Add the physics mode radio buttons
		const float Y_POS = 0.25f;
		const float DIMS_PIXELS = 20.0f;
		const float PADDING = 0.04f;

		m_rigidRadioButton = static_cast<CEGUI::RadioButton*>(m_gui.createWidget("TaharezLook/RadioButton", glm::vec4(0.05f, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS),"RigidButton")); // here the destRectPixels size is used instead of the destRectPercentage size
		m_rigidRadioButton->setSelected(true);
		m_rigidRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onRigidMouseClick, this));


		m_dynamicRadioButton = static_cast<CEGUI::RadioButton*>(m_gui.createWidget("TaharezLook/RadioButton", glm::vec4(0.05f + PADDING, Y_POS, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, DIMS_PIXELS, DIMS_PIXELS), "DynamicButton")); // here the destRectPixels size is used instead of the destRectPercentage size
		m_dynamicRadioButton->setSelected(false);
		m_dynamicRadioButton->subscribeEvent(CEGUI::RadioButton::EventMouseClick, CEGUI::Event::Subscriber(&EditorScreen::onDynamicMouseClick, this));
	}

	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	// need to injectMousePosition to show, which is done in GUI.cpp onSDLEvent which allows for GUIs to become interactive
	m_gui.showMouseCursor();
	// disables the default mouse cursor so only see the CEGUI mouse cursor, 1 means to show the cursor and 0 means to not show the cursor
	SDL_ShowCursor(0);

}

void EditorScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);
		switch (evnt.type) {
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs()); // creates some temporary args 
			break;
		}
	}
}



bool EditorScreen::onExitClicked(const CEGUI::EventArgs & e) {
	m_currentState = Bengine::ScreenState::EXIT_APPLICATION;
	return true;
}



bool EditorScreen::onColorPickerRedChanged(const CEGUI::EventArgs & e) {
	m_colorPickerRed = m_rSlider->getCurrentValue();
	return true;
}



bool EditorScreen::onColorPickerGreenChanged(const CEGUI::EventArgs & e) {
	m_colorPickerGreen = m_gSlider->getCurrentValue();
	return true;
}



bool EditorScreen::onColorPickerBlueChanged(const CEGUI::EventArgs & e) {
	m_colorPickerBlue = m_bSlider->getCurrentValue();
	return true;
}



bool EditorScreen::onRigidMouseClick(const CEGUI::EventArgs & e) {
	m_physicsMode = PhysicsMode::RIGID;
	return true;
}



bool EditorScreen::onDynamicMouseClick(const CEGUI::EventArgs & e) {
	m_physicsMode = PhysicsMode::DYNAMIC;
	return true;
}



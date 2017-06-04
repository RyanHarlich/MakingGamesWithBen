#include "MainMenuScreen.h"

/* NEW: moved to header file */
//#include "ScreenIndices.h"

MainMenuScreen::MainMenuScreen(Bengine::Window * window) : m_window(window) {
	m_screenIndex = SCREEN_INDEX_MAINMENU;
}

MainMenuScreen::~MainMenuScreen() {
}

int MainMenuScreen::getNextScreenIndex() const {
	return m_nextScreenIndex;
}

int MainMenuScreen::getPreviousScreenIndex() const {
	return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::build() {
}

void MainMenuScreen::destroy() {
}

void MainMenuScreen::onEntry() {
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
	m_camera.setScale(32.0); // 32 pixels per meter

	initUI();
}

void MainMenuScreen::onExit() {
	m_gui.destroy();
}

void MainMenuScreen::update() {
	m_camera.update();
	checkInput();
}

void MainMenuScreen::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Dark blue background

	m_gui.draw();
}

void MainMenuScreen::initUI() {

	// Init the UI, call this after initializing all the openGL stuff and glew 
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");


	/* NEW: forgot to remove this */
	//m_gui.loadScheme("AlfiskoSkin.scheme");


	m_gui.setFont("DejaVuSans-10");
	


	// New Game Button
	CEGUI::PushButton* playGameButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "NewGameButton")); // .1 / .2 is .5 so .45 will center correctly, look at notes for details
	playGameButton->setText("New Game");
	// Set up event to be called when we click
	playGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onNewGameClicked, this));





	/* NEW */
	// Editor Button
	CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.56f, 0.1f, 0.05f), glm::vec4(0.0f), "EditorButton")); // .1 / .2 is .5 so .45 will center correctly, look at notes for details
	editorButton->setText("Level Editor");
	// Set up event to be called when we click
	editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onEditorClicked, this));
	/* NEW: end of new */



	

	// Exit Button
	// do not include the .font for font
	// Change between the AlfiskoSkin and the TaharezLook by changing the part in front of ".../Button"
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.45f, 0.62f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton")); // 50%x 50%y is in the middle, then the dimensions follow // pixels do not matter as of now so set to 0 // .1 / .2 is .5 so .45 will center correctly, look at notes for details// .6 will bring it down the screen to 60% 

	exitButton->setText("Exit Game");

	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));


	m_gui.setMouseCursor("TaharezLook/MouseArrow");
	// need to injectMousePosition to show, which is done in GUI.cpp onSDLEvent which allows for GUIs to become interactive
	m_gui.showMouseCursor();
	// disables the default mouse cursor so only see the CEGUI mouse cursor, 1 means to show the cursor and 0 means to not show the cursor
	SDL_ShowCursor(0);

}

void MainMenuScreen::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_gui.onSDLEvent(evnt);

		/* NEW */
		switch (evnt.type) {
		case SDL_QUIT:
			onExitClicked(CEGUI::EventArgs());
			break;
		}
		/* NEW: end of new */


	}
}



// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
bool MainMenuScreen::onNewGameClicked(const CEGUI::EventArgs & e) {

	/* NEW */
	m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;

	m_currentState = Bengine::ScreenState::CHANGE_NEXT;
	return true;
}



/* NEW */
// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
bool MainMenuScreen::onEditorClicked(const CEGUI::EventArgs & e) {
	m_nextScreenIndex = SCREEN_INDEX_EDITOR;
	m_currentState = Bengine::ScreenState::CHANGE_NEXT;
	return true;
}



// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs & e) {
	m_currentState = Bengine::ScreenState::EXIT_APPLICATION;
	return true;
}

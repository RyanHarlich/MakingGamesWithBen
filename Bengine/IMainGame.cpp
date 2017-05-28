#include "IMainGame.h"
#include "Timing.h"
#include "ScreenList.h"
#include "IGameScreen.h"


namespace Bengine {



	IMainGame::IMainGame() {
		m_screenList = std::make_unique<ScreenList>(this);
	}




	IMainGame::~IMainGame() {
		// Empty
	}




	void IMainGame::run() {

		if (!init()) return;

		FpsLimiter limiter;
		limiter.init(60.0f);
		limiter.begin();

		// Game loop
		m_isRunning = true;
		while (m_isRunning) {
			limiter.begin();

			// Call the custom update and draw method
			inputManager.update();
			update();
			
			draw();

			m_fps = limiter.end();
			m_window.swapBuffer();
		}

	}





	void IMainGame::exitGame() {
		m_currentScreen->onExit();
		if (m_screenList) {
			m_screenList->destory();
			m_screenList.reset();
		}
		m_isRunning = false;
	}





	void IMainGame::update() {
		if (m_currentScreen) {
			switch (m_currentScreen->getState()) {
			case ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			case ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
				if (m_currentScreen) {
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			case ScreenState::EXIT_APPLICATION:
				exitGame();
				break;
			default:
				break;
			}
		}
		else {
			exitGame();
		}
	}





	void IMainGame::draw() {
		glViewport(0, 0, m_window.getScreenWidth(), m_window.getScreenHeight());
		if (m_currentScreen && m_currentScreen->getState() == ScreenState::RUNNING) {
			m_currentScreen->draw();
		}
	}





	void IMainGame::onSDLEvent(SDL_Event & evnt) {
		switch (evnt.type) {
		case SDL_QUIT:
			/* NEW: trying to destroy a screen that has already been freed (in screenlist.cpp destroy function), the screen does not live in screenlist, there is a pointer to it in screenlist, but the screens memory is stored in the app, and app is getting destoryed before calling the destructor for screen list. Instead call exitGame which will do the destruction and set m_isRunning to false. It seems   */
			exitGame();
			//m_isRunning = false;
			break;
		case SDL_MOUSEMOTION:
			inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputManager.releaseKey(evnt.button.button);
			break;
		}
	}





	bool IMainGame::init() {
		
		Bengine::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		if (!initSystems()) return false;

		onInit();
		addScreens();

		m_currentScreen = m_screenList->getCurrent();
		m_currentScreen->onEntry();
		m_currentScreen->setRunning();

		return true;
	}




	bool IMainGame::initSystems() {
		m_window.create("Default", 1476, 876, 0);
		//m_window.create("Default", 1920, 1080, 0);
		return true;
	}

}
#include "IMainGame.h"
#include "Timing.h"

#include "ScreenList.h"
#include "IGameScreen.h"


namespace Bengine {



	IMainGame::IMainGame() {
		// Empty
	}




	IMainGame::~IMainGame() {
		// Empty
	}




	void IMainGame::run() {

		if (!init()) return;

		FpsLimiter limiter;
		limiter.init(60.0f);
		limiter.begin();

		m_isRunning = true;
		while (m_isRunning) {
			limiter.begin();

			update();
			draw();

			m_fps = limiter.end();
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





	bool IMainGame::init() {
		
		Bengine::init();
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		initSystems();

		onInit();
	}




	bool IMainGame::initSystems() {
		m_window.create("Default", 1920, 1080, 0);
	}

}
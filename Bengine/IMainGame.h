#pragma once

#include <memory>

#include "Window.h"
#include "Bengine.h"
#include "InputManager.h"


namespace Bengine {

	class IGameScreen;
	class ScreenList;

	class IMainGame {
	public:
		IMainGame();
		virtual ~IMainGame();

		// Runs and initializes the game
		void run();
		// Exits the game
		void exitGame();

		// Called on initialization 
		virtual void onInit() = 0;
		// For adding all screens
		virtual void addScreens() = 0;
		// Called when exiting
		virtual void onExit() = 0;

		void onSDLEvent(SDL_Event& evnt);

		const float getFps() const { return m_fps; }

		/* NEW: public variables are usually bad */
		InputManager inputManager;

	protected:
		// Custom update function
		virtual void update();
		// Custom render function
		virtual void draw();

		
		bool init();
		bool initSystems();

		std::unique_ptr<ScreenList> m_screenList = nullptr;
		IGameScreen* m_currentScreen = nullptr;
		bool m_isRunning = false;
		float m_fps = 0.0f;
		Window m_window;

		/* NEW: moved to become a public variable */
		//InputManager inputManager;

	};

}
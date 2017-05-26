#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

namespace Bengine {

	class MainGame;

	enum class ScreenState {
		NONE,
		RUNNING,
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IGameScreen
	{
	public:
		IGameScreen() { // Empty
		}
		virtual ~IGameScreen() { // Empty 
		}

		// Return the index of the next or previous screen when changing screens
		virtual int getNextScreenIndex() const = 0;
		virtual int getPreviousScreenIndex() const = 0;

		// Called at beginning and end of application
		virtual void build() = 0;
		virtual void destroy() = 0;

		// Called when a screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		// Called in the main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		int getScreenIndex() const { return m_screenIndex; }

		void setParentGame(MainGame* game) { m_game = game; }




	protected:
		ScreenState m_currentState = ScreenState::NONE;
		MainGame* m_game = nullptr;
		int m_screenIndex = SCREEN_INDEX_NO_SCREEN;



	};

}
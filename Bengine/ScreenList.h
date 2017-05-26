#pragma once

#include <vector>

#include "IGameScreen.h"


namespace Bengine {

	class IMainGame;

	class ScreenList
	{
	public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destory();

		

		IGameScreen* getCurrent();

	protected:
		IMainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	};

}
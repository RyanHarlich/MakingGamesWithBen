#pragma once

#include <vector>

class MainGame;
class IGameScreen;

namespace Bengine {

	class ScreenList
	{
	public:
		ScreenList(MainGame* game);

		IGameScreen* moveNext();
		IGameScreen* movePrevious();

		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destory();

		~ScreenList();

		IGameScreen* getCurrent();

	protected:
		MainGame* m_game = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	};

}
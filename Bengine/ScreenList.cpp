#include "ScreenList.h"
#include "IGameScreen.h"

namespace Bengine {

	ScreenList::ScreenList(MainGame* game) : m_game(game) {
		//Empty
	}



	IGameScreen* ScreenList::moveNext() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getNextScreenIndex();
		}
		return getCurrent();
	}



	IGameScreen* ScreenList::movePrevious() {
		IGameScreen* currentScreen = getCurrent();
		if (currentScreen->getPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
			m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
		}
		return getCurrent();
	}



	void ScreenList::setScreen(int nextScreen) {
		m_currentScreenIndex = nextScreen;
	}



	void ScreenList::addScreen(IGameScreen* newScreen) {
		m_screens.push_back(newScreen);
		newScreen->build();
		newScreen->setParentGame(m_game);
	}



	void ScreenList::destory() {
		for (size_t i = 0; i < m_screens.size(); ++i) {
			m_screens[i]->destroy();
			delete m_screens[i];
		}
		m_screens.resize(0);
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
	}




	ScreenList::~ScreenList() {
		destory();
	}



	IGameScreen* ScreenList::getCurrent() {
		if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
		return m_screens[m_currentScreenIndex];
	}

}
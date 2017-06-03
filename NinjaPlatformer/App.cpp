#include "App.h"

#include <Bengine/ScreenList.h>


App::App()
{
}


App::~App()
{
}



void App::onInit() {
}



void App::addScreens() {

	/* NEW */
	m_mainMenuScreen = std::make_unique<MainMenuScreen>(&m_window);

	m_gameplayScreen = std::make_unique<GameplayScreen>(&m_window);

	/* NEW: make sure these are in order, where mainmenuscreen is 0 and gameplayscreen is 1 */
	m_screenList->addScreen(m_mainMenuScreen.get());

	m_screenList->addScreen(m_gameplayScreen.get());

	/* NEW: set to mainmenuscreen instead */
	//m_screenList->setScreen(m_gameplayScreen->getScreenIndex());
	m_screenList->setScreen(m_mainMenuScreen->getScreenIndex());

}



void App::onExit() {
}



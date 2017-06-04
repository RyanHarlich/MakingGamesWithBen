#pragma once

#include "GameplayScreen.h"
#include "MainMenuScreen.h"

/* NEW */
#include "EditorScreen.h"

#include <Bengine/IMainGame.h>



class App : public Bengine::IMainGame {

	public:
		App();
		~App();

		// Called on initialization 
		virtual void onInit() override;
		// For adding all screens
		virtual void addScreens() override;
		// Called when exiting
		virtual void onExit() override;
private:
	std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;

	/* NEW */
	std::unique_ptr<EditorScreen> m_editorScreen = nullptr;

};



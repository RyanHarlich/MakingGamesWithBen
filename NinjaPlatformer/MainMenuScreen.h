#pragma once

/* NEW */
#include "ScreenIndices.h"

#include <Bengine/IGameScreen.h>
#include <Bengine/Window.h>
#include <Bengine/GUI.h>
#include <Bengine/Camera2D.h>

class MainMenuScreen : public Bengine::IGameScreen
{
public:
	MainMenuScreen(Bengine::Window* window);

	~MainMenuScreen();

	// Return the index of the next or previous screen when changing screens
	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	// Called at beginning and end of application
	virtual void build() override;
	virtual void destroy() override;

	// Called when a screen enters and exits focus
	virtual void onEntry() override;
	virtual void onExit() override;

	// Called in the main game loop
	virtual void update() override;
	virtual void draw() override;

private:
	void initUI();
	void checkInput();
	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	bool onNewGameClicked(const CEGUI::EventArgs& e);

	/* NEW */
	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	bool MainMenuScreen::onEditorClicked(const CEGUI::EventArgs & e);

	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	bool onExitClicked(const CEGUI::EventArgs& e);


	/* NEW */
	int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;

	Bengine::Camera2D m_camera;
	Bengine::Window* m_window;
	Bengine::GUI m_gui;
};
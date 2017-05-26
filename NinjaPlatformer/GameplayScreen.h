#pragma once

#include <Bengine/IGameScreen.h>

class GameplayScreen : public Bengine::IGameScreen
{
public:
	GameplayScreen();
	~GameplayScreen();

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
	void checkInput();


};


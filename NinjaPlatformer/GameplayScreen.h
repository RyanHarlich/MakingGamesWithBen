#pragma once

#include <memory>

/* NEW */
#include <vector>

/* NEW */
#include "Box.h"

#include <Bengine/IGameScreen.h>

/* NEW */
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>

/* NEW */
#include <Box2D/Box2D.h>


class GameplayScreen : public Bengine::IGameScreen
{
public:

	/* NEW: paramter and initialization list */
	GameplayScreen(Bengine::Window* window);

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

	/* NEW*/
	Bengine::SpriteBatch m_spriteBatch;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::Camera2D m_camera;
	Bengine::GLTexture m_texture;
	Bengine::Window* m_window;

	/* NEW */
	std::unique_ptr<b2World> m_world;
	/* NEW: do not need pointers because the memory is allocated inside the world */
	std::vector<Box> m_boxes;
	


};


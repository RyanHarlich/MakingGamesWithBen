#pragma once

#include <memory>
#include <vector>

#include "Box.h"
#include "Player.h"

#include <Bengine/IGameScreen.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Box2D/Box2D.h>
#include <Bengine/DebugRenderer.h>
#include <Bengine/GUI.h>


class GameplayScreen : public Bengine::IGameScreen
{
public:
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


	Bengine::SpriteBatch m_spriteBatch;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::GLSLProgram m_lightProgram;
	Bengine::Camera2D m_camera;
	Bengine::GLTexture m_texture;
	Bengine::Window* m_window;
	Bengine::DebugRenderer m_debugRenderer;
	Bengine::GUI m_gui;
	bool m_renderDebug = false;
	Player m_player;
	std::unique_ptr<b2World> m_world;
	std::vector<Box> m_boxes;
	


};


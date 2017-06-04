#pragma once


#include <Bengine/IGameScreen.h>
#include <Bengine/Window.h>
#include <Bengine/GUI.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/GLTexture.h>
#include <Bengine/SpriteFont.h>


enum class PhysicsMode {
	RIGID,
	DYNAMIC
};

class EditorScreen : public Bengine::IGameScreen {
public:
	EditorScreen(Bengine::Window* window);

	~EditorScreen();

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
	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);

	
	float m_colorPickerRed = 255.0f;
	float m_colorPickerGreen = 255.0f;
	float m_colorPickerBlue = 255.0f;

	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
	CEGUI::RadioButton* m_rigidRadioButton = nullptr;
	CEGUI::RadioButton* m_dynamicRadioButton = nullptr;

	Bengine::GLTexture m_blankTexture;

	PhysicsMode m_physicsMode = PhysicsMode::RIGID;

	Bengine::SpriteBatch m_spriteBatch;
	Bengine::SpriteFont m_spriteFont;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::Camera2D m_camera;
	Bengine::Window* m_window;
	Bengine::GUI m_gui;
	
};

#pragma once

#include <memory>

#include "Player.h"
#include "Box.h"
#include "Light.h"


#include <Bengine/IGameScreen.h>
#include <Bengine/Window.h>
#include <Bengine/GUI.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/GLTexture.h>
#include <Bengine/SpriteFont.h>


#include <Box2D/Box2D.h>






enum class PhysicsMode {
	RIGID,
	DYNAMIC
};





enum class ObjectMode {
	PLAYER,
	PLATFORM,
	LIGHT,
	FINISH
};


enum class SelectionMode {
	SELECT,
	PLACE
};



const int NO_BOX = -1;
const int NO_LIGHT = -1;



class WidgetLabel {
public:
	WidgetLabel() {};
	WidgetLabel(CEGUI::Window* widget, const std::string& text, float scale) :
		widget(widget), text(text), scale(scale) {

	}
	void draw(Bengine::SpriteBatch& sb, Bengine::SpriteFont& sf, Bengine::Window* w);

	CEGUI::Window* widget = nullptr;
	std::string text = "";
	Bengine::ColorRGBA8 color = Bengine::ColorRGBA8(255, 255, 255, 255);
	float scale = 0.7f;
};



class EditorScreen : public Bengine::IGameScreen {
public:
	EditorScreen(Bengine::Window* window);

	~EditorScreen();

	/***************************************************************************/
	/*   IGameScreen Implementation                                            */
	/***************************************************************************/
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


	void drawUI();

	void drawWorld();


	void initUI();


	void clearLevel();


	void checkInput();


	void updateMouseDown(const SDL_Event& evnt);
	void updateMouseUp(const SDL_Event& evnt);
	void updateMouseMotion(const SDL_Event& evnt);
	void refreshSelectedBox();
	void refreshSelectedBox(const glm::vec2& newPosition);
	void refreshSelectedLight();
	void refreshSelectedLight(const glm::vec2& newPosition);
	bool isMouseInUI();


	void setPlatformWidgetVisibility(bool visible);
	void setLightWidgetVisibility(bool visible);






	/***************************************************************************/
	/*   Event Handlers                                                        */
	/***************************************************************************/
	// type of function for function pointer is a std::function<bool(const CEGUI::EventArgs&)>
	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onColorPickerRedChanged(const CEGUI::EventArgs& e);
	bool onColorPickerGreenChanged(const CEGUI::EventArgs& e);
	bool onColorPickerBlueChanged(const CEGUI::EventArgs& e);
	bool onRigidMouseClick(const CEGUI::EventArgs& e);
	bool onDynamicMouseClick(const CEGUI::EventArgs& e);
	bool onColorPickerAlphaChanged(const CEGUI::EventArgs& e);
	bool onPlayerMouseClick(const CEGUI::EventArgs& e);
	bool onPlatformMouseClick(const CEGUI::EventArgs& e);
	bool onFinishMouseClick(const CEGUI::EventArgs& e);
	bool onLightMouseClick(const CEGUI::EventArgs& e);
	bool onSelectMouseClick(const CEGUI::EventArgs& e);
	bool onPlaceMouseClick(const CEGUI::EventArgs& e);
	bool onSaveMouseClick(const CEGUI::EventArgs& e);
	bool onBackMouseClick(const CEGUI::EventArgs& e);
	bool onRotationValueChange(const CEGUI::EventArgs& e);
	bool onSizeValueChange(const CEGUI::EventArgs& e);
	bool onWidthValueChange(const CEGUI::EventArgs& e);
	bool onHeightValueChange(const CEGUI::EventArgs& e);
	bool onDebugToggleClick(const CEGUI::EventArgs& e);
	bool onSave(const CEGUI::EventArgs& e);
	bool onSaveCancelClick(const CEGUI::EventArgs& e);
	bool onLoadCancelClick(const CEGUI::EventArgs& e);
	bool onLoadMouseClick(const CEGUI::EventArgs& e);
	bool onLoad(const CEGUI::EventArgs& e);


	/***************************************************************************/
	/*   Member Variables                                                      */
	/***************************************************************************/
	float m_colorPickerRed = 255.0f;
	float m_colorPickerGreen = 255.0f;
	float m_colorPickerBlue = 255.0f;
	float m_colorPickerAlpha = 255.0f;
	float m_rotation = 0.0f;
	float m_lightSize = 10.0f;
	glm::vec2 m_boxDims = glm::vec2(1.0f);
	bool m_mouseButtons[2];
	bool m_debugRender = false;



	CEGUI::GroupBox* m_groupBox;
	CEGUI::Slider* m_rSlider = nullptr;
	CEGUI::Slider* m_gSlider = nullptr;
	CEGUI::Slider* m_bSlider = nullptr;
	CEGUI::Slider* m_aSlider = nullptr;
	CEGUI::RadioButton* m_rigidRadioButton = nullptr;
	CEGUI::RadioButton* m_dynamicRadioButton = nullptr;
	CEGUI::RadioButton* m_playerRadioButton = nullptr;
	CEGUI::RadioButton* m_platformRadioButton = nullptr;
	CEGUI::RadioButton* m_finishRadioButton = nullptr;
	CEGUI::RadioButton* m_lightRadioButton = nullptr;
	CEGUI::RadioButton* m_selectRadioButton = nullptr;
	CEGUI::RadioButton* m_placeRadioButton = nullptr;
	CEGUI::ToggleButton* m_debugToggle = nullptr;
	CEGUI::Spinner* m_rotationSpinner = nullptr;
	CEGUI::Spinner* m_sizeSpinner = nullptr;
	CEGUI::Spinner* m_widthSpinner = nullptr;
	CEGUI::Spinner* m_heightSpinner = nullptr;
	CEGUI::PushButton* m_loadButton = nullptr;
	CEGUI::PushButton* m_saveButton = nullptr;
	CEGUI::PushButton* m_backButton = nullptr;
	std::vector<CEGUI::ListboxTextItem*> m_saveListBoxItems;
	CEGUI::FrameWindow* m_saveWindow = nullptr;
	CEGUI::PushButton* m_saveWindowSaveButton = nullptr;
	CEGUI::Combobox* m_saveWindowCombobox = nullptr;
	std::vector<CEGUI::ListboxTextItem*> m_loadListBoxItems;
	CEGUI::FrameWindow* m_loadWindow = nullptr;
	CEGUI::PushButton* m_loadWindowLoadButton = nullptr;
	CEGUI::Combobox* m_loadWindowCombobox = nullptr;
	std::vector<WidgetLabel> m_widgetLabels;







	Bengine::GLTexture m_blankTexture;

	PhysicsMode m_physicsMode = PhysicsMode::RIGID;
	ObjectMode m_objectMode = ObjectMode::PLAYER;
	SelectionMode m_selectMode = SelectionMode::SELECT;


	Bengine::SpriteBatch m_spriteBatch;
	Bengine::SpriteFont m_spriteFont;
	Bengine::GLSLProgram m_textureProgram;
	Bengine::GLSLProgram m_lightProgram;
	Bengine::DebugRenderer m_debugRenderer;



	bool m_hasPlayer = false;
	Player m_player;
	std::vector<Box> m_boxes;
	std::vector<Light> m_lights;
	int m_selectedBox = NO_BOX;
	int m_selectedLight = NO_LIGHT;
	bool m_isDragging = false;
	glm::vec2 m_selectOffset;



	Bengine::Camera2D m_uiCamera;
	Bengine::InputManager m_inputManager;
	Bengine::Camera2D m_camera;
	Bengine::Window* m_window;
	Bengine::GUI m_gui;


	std::unique_ptr<b2World> m_world;
	
};

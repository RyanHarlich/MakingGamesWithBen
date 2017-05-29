#pragma once

#include "Box.h"
#include "Capsule.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>

/* NEW */
#include <Bengine/TileSheet.h>

/* NEW: removed */
//#include <Bengine/GLTexture.h>

/* NEW */
enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };


/* NEW */
#define RIGHT 1
#define LEFT -1


class Player {
public:


	void init(b2World* world, 
		const glm::vec2 position, 
		const glm::vec2 drawDims, 
		const glm::vec2 collisionDims, 
		Bengine::ColorRGBA8 color, 
		bool fixedRotation);

	void draw(Bengine::SpriteBatch& spriteBatch);

	void drawDebug(Bengine::DebugRenderer& debugRenderer);

	void update(Bengine::InputManager& inputManager);

	//Getter
	const Capsule& getCapsule() const { return m_capsule; }


private:
	glm::vec2 m_drawDims;
	Bengine::ColorRGBA8 m_color;

	/* NEW: removed */
	//Bengine::GLTexture m_texture;
	/* NEW */
	Bengine::TileSheet m_texture;

	Capsule m_capsule;

	/* NEW */
	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	float m_animTime = 0.0f;
	int m_direction = RIGHT;
	bool m_onGround = false;
	bool m_isPunching = false; // this is for kicking or punching

};
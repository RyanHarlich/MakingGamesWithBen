#pragma once

#include "Box.h"
#include "Capsule.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/TileSheet.h>



enum class PlayerMoveState { STANDING, RUNNING, PUNCHING, IN_AIR };

enum class Direction { LEFT, RIGHT };


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

	glm::vec2 getPosition() const { 
		return glm::vec2(m_capsule.getBody()->GetPosition().x, m_capsule.getBody()->GetPosition().y); 
	}


private:
	glm::vec2 m_drawDims;
	Bengine::ColorRGBA8 m_color;
	Bengine::TileSheet m_texture;
	Capsule m_capsule;
	PlayerMoveState m_moveState = PlayerMoveState::STANDING;
	float m_animTime = 0.0f;
	Direction m_direction = Direction::RIGHT;
	bool m_onGround = false;
	bool m_isPunching = false; // this is for kicking or punching

};
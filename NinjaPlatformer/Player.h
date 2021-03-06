#pragma once

#include "Capsule.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/TileSheet.h>
#include <Bengine/AudioEngine.h>


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



	void initSoundEffects();
	void destorySoundEffects();


	void destory(b2World* world);



	void draw(Bengine::SpriteBatch& spriteBatch);

	void drawDebug(Bengine::DebugRenderer& debugRenderer);

	void update(Bengine::InputManager& inputManager);

	//Getter
	const Capsule& getCapsule() const { return m_capsule; }

	glm::vec2 getPosition() const { 
		return glm::vec2(m_capsule.getBody()->GetPosition().x, m_capsule.getBody()->GetPosition().y); 
	}


	const glm::vec2				getCollisionDims()	const { return m_collisionDims; }
	const glm::vec2				getDrawDims()		const { return m_drawDims; }
	const Bengine::ColorRGBA8	getColor()			const { return m_color; }



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
	glm::vec2 m_collisionDims;



	Bengine::AudioEngine m_audio;
	Bengine::SoundEffect m_soundKick;
	Bengine::SoundEffect m_soundPunch;

};
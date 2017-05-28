#pragma once

#include "Box.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/GLTexture.h>

/* NEW */
#include <Bengine/InputManager.h>

class Player {
public:
	Player();
	~Player();

	void init(b2World * world, const glm::vec2 position, const glm::vec2 dimensions, Bengine::ColorRGBA8 color, bool fixedRotation);

	void draw(Bengine::SpriteBatch& spriteBatch);

	void update(Bengine::InputManager& inputManager);

	//Getter
	const Box& getBox() const { return m_collisionBox; }

private:
	Box m_collisionBox;

};
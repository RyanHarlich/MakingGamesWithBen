#pragma once

#include "Box.h"

/* NEW */
#include "Capsule.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/GLTexture.h>
#include <Bengine/InputManager.h>

class Player {
public:

	/* NEW: removed dimensions and added collision dimensions and draw dimensions */
	void init(b2World* world, 
		const glm::vec2 position, 
		const glm::vec2 drawDims, 
		const glm::vec2 collisionDims, 
		Bengine::ColorRGBA8 color, 
		bool fixedRotation);

	void draw(Bengine::SpriteBatch& spriteBatch);

	/* NEW */
	void drawDebug(Bengine::DebugRenderer& debugRenderer);

	void update(Bengine::InputManager& inputManager);

	//Getter
	/* NEW: change to get capsule instead of box for player */
	//const Box& getBox() const { return m_collisionBox; }
	const Capsule& getCapsule() const { return m_capsule; }


private:
	/* NEW */
	glm::vec2 m_drawDims;
	Bengine::ColorRGBA8 m_color;
	Bengine::GLTexture m_texture;

	/* NEW: replace with a capsule for the player */
	//Box m_collisionBox;
	Capsule m_capsule;

};
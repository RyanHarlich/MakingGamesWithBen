#include "Player.h"

#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>


/* NEW: removed dimensions and added collision dimensions and draw dimensions */
void Player::init(b2World * world, 
	const glm::vec2 position, 
	const glm::vec2 drawDims,
	const glm::vec2 collisionDims, 
	Bengine::ColorRGBA8 color, 
	bool fixedRotation) {

	/* NEW: changed to m_texture */
	//Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/Ninja/blue_ninja.png");
	m_texture = Bengine::ResourceManager::getTexture("Textures/Ninja/blue_ninja.png");

	/* NEW */
	m_color = color;
	m_drawDims = drawDims;


	/* NEW: changed to capsule instead of a box for player */
	//m_collisionBox.init(world, position, dimensions, texture, color, fixedRotation, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f));
	m_capsule.init(world, position, collisionDims, 1.0f, 0.1f, fixedRotation);
}






void Player::draw(Bengine::SpriteBatch& spriteBatch) {

	/* NEW: removed */
	//m_collisionBox.draw(spriteBatch);
	
	/* NEW: added draw code */
	glm::vec4 destRect;
	b2Body* body = m_capsule.getBody();
	// The position of the box is actually the center of the box not the corner of the box
	destRect.x = body->GetPosition().x - (m_drawDims.x / 2.0f);
	destRect.y = body->GetPosition().y - (m_capsule.getDimensions().y / 2.0f); // this should be m_capsule get dimensions so the sprites feet are perfectly aligned with the bottom of the collision box
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;
	spriteBatch.draw(destRect, glm::vec4(0.0f, 0.0f, 0.1f, 0.5f), m_texture.id, 0.0f, m_color, body->GetAngle());
	/* NEW: end of new */

}








void Player::drawDebug(Bengine::DebugRenderer& debugRenderer){
	m_capsule.drawDebug(debugRenderer);
}







void Player::update(Bengine::InputManager& inputManager) {

	// Get body of player
	/* NEW: changed to capsule instead of box for player */
	//b2Body* body = m_collisionBox.getBody();
	b2Body* body = m_capsule.getBody();


	if (inputManager.isKeyDown(SDLK_a) || inputManager.isKeyDown(SDLK_LEFT)) {
		// apply force to object, wake up object (objects can go to sleep)
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
	}
	else if (inputManager.isKeyDown(SDLK_d) || inputManager.isKeyDown(SDLK_RIGHT)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
	}
	else {
		// Apply damping: slow down velocity kind of like friction
		// reduce velocity, to avoid floating, by 5% (only keep 95% of the x velocity) in the x directions
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));

	}



	static const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED){
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}
	


	// Loop through all the contact points
	// contact edge, get contact list is a linked list that needs a special type of iteration
	for (b2ContactEdge* ce = body->GetContactList(); ce != nullptr; ce = ce->next) {

		// The actual contact that is colliding with player
		// the actual point that is touching the player
		b2Contact* c = ce->contact;

		// if contact is touching player
		if (c->IsTouching()) {

			// stores all contact points of specific edge
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold); // pointer gets full of manifold data 
			// Check if the points are below
			bool below = false;
			//maxManifoldPoints is two contact points for all convex shapes
			for (int i = 0; i < b2_maxManifoldPoints; ++i) {
				// if monifold points y is less than the center of the player minus half its y dimension

				/* NEW: changed to capsule instead of box for player, also changed the .01 to half the capsules width (this is explained in tutorial 50 so not sure if there is a typo somewhere else because in the video there was not a jumping problem */
				//if (manifold.points[i].y < body->GetPosition().y - (m_collisionBox.getDimensions().y / 2.0f) + 0.01f) {
				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2.0f) {

					below = true;

					// this break will break out of the loop since only needs to be touching once to jump
					break;
				}
			}
			if (below) {
				// Play can jump
				if (inputManager.isKeyPressed(SDLK_w) || inputManager.isKeyPressed(SDLK_UP) || inputManager.isKeyPressed(SDLK_SPACE)) {
					// apply force to object, wake up object (objects can go to sleep)
					// 2nd argument does not matter, asking for point of center
					body->ApplyLinearImpulse(b2Vec2(0.0f, 30.0f), b2Vec2(0.0f, 0.0f), true);

					// this break will break out of the loop to avoid a high jump in case player is touching two boxes
					break;
				}
			}
		}
	}


}



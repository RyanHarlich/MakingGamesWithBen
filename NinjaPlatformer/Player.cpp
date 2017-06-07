#include "Player.h"

#include <Bengine/ResourceManager.h>

#include <SDL/SDL.h>



void Player::init(b2World * world, 
	const glm::vec2 position, 
	const glm::vec2 drawDims,
	const glm::vec2 collisionDims, 
	Bengine::ColorRGBA8 color, 
	bool fixedRotation) {


	Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/Ninja/blue_ninja.png");

	m_color = color;
	m_drawDims = drawDims;
	m_collisionDims = collisionDims;

	m_capsule.init(world, position, collisionDims, 1.0f, 0.1f, fixedRotation);


	m_texture.init(texture, glm::ivec2(10, 2));
}





void Player::destory(b2World * world){
	m_capsule.destroy(world);
}






void Player::draw(Bengine::SpriteBatch& spriteBatch) {

	glm::vec4 destRect;
	b2Body* body = m_capsule.getBody();
	// The position of the box is actually the center of the box not the corner of the box
	destRect.x = body->GetPosition().x - (m_drawDims.x / 2.0f);
	destRect.y = body->GetPosition().y - (m_capsule.getDimensions().y / 2.0f); // this should be m_capsule get dimensions so the sprites feet are perfectly aligned with the bottom of the collision box
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;



	int tileIndex;
	int numTiles;

	float animSpeed = 0.2f; // animation speed
	glm::vec2 velocity;
	velocity.x = body->GetLinearVelocity().x;
	velocity.y = body->GetLinearVelocity().y;

	// Calculate animation
	if (m_onGround) {

		if (m_isPunching) {
			// Punching
			numTiles = 4;
			tileIndex = 1;

			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}


		} else if (abs(velocity.x) > 1.0f && ((velocity.x > 0 && m_direction == Direction::RIGHT) || (velocity.x < 0 && m_direction == Direction::LEFT))) { // absolute of velocity says to ignore direction
			// Running
			numTiles = 6;
			tileIndex = 10;
			animSpeed = abs(velocity.x) * 0.025f; // this will cause legs to slow down when velocity slows down
			if (m_moveState != PlayerMoveState::RUNNING) {
				m_moveState = PlayerMoveState::RUNNING;
				m_animTime = 0.0f;
			}
		}
		else { // Standing still
			numTiles = 1;
			tileIndex = 0;
			m_moveState = PlayerMoveState::STANDING;
		}
	}
	else {
		// In the air
		if (m_isPunching) {
			// Kicking
			numTiles = 1;
			tileIndex = 18;
			animSpeed *= 0.25;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animTime = 0.0f;
			}
		}
		else if (abs(velocity.x) > 10.0f) {
			numTiles = 1;
			tileIndex = 10;
			m_moveState = PlayerMoveState::IN_AIR;
		}
		else if (velocity.y <= 0.0f) {
			// Falling
			numTiles = 1;
			tileIndex = 17;
			m_moveState = PlayerMoveState::IN_AIR;
		}
		else {
			// Rising
			numTiles = 1;
			tileIndex = 16;
			m_moveState = PlayerMoveState::IN_AIR;
		}
	}

	// Increment animation time
	m_animTime += animSpeed;

	// Check for punch end
	if (m_animTime > numTiles) {
		m_isPunching = false;
	}

	// Apply animation
	tileIndex = tileIndex + (int)m_animTime % numTiles; // starts at tile index and gives remainder of the number of tiles


	// Get the uv coordinates from the tile index
	glm::vec4 uvRect = m_texture.getUVs(tileIndex);

	// Check direction
	if (m_direction == Direction::LEFT) {
		uvRect.x += 1.0f / m_texture.dims.x; // this will add one tile over
		uvRect.z *= -1; // reverese the direction of the uv coordinates, (z is width), in the shader it will be a negative number for the demensions which will cause to go back one tile but backwards
	}




	// draw the sprite
	spriteBatch.draw(destRect, uvRect, m_texture.texture.id, 0.0f, m_color, body->GetAngle());
}








void Player::drawDebug(Bengine::DebugRenderer& debugRenderer){
	m_capsule.drawDebug(debugRenderer);
}







void Player::update(Bengine::InputManager& inputManager) {

	// Get body of player
	b2Body* body = m_capsule.getBody();


	if (inputManager.isKeyDown(SDLK_a) || inputManager.isKeyDown(SDLK_LEFT)) {
		// apply force to object, wake up object (objects can go to sleep)
		body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
		m_direction = Direction::LEFT;
	}
	else if (inputManager.isKeyDown(SDLK_d) || inputManager.isKeyDown(SDLK_RIGHT)) {
		body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
		m_direction = Direction::RIGHT;
	}
	else {
		// Apply damping: slow down velocity kind of like friction
		// reduce velocity, to avoid floating, by 5% (only keep 95% of the x velocity) in the x directions
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.95f, body->GetLinearVelocity().y));
	}


	// check for punch
	if (inputManager.isKeyPressed(SDLK_SPACE)) {
		m_isPunching = true;
	}


	static const float MAX_SPEED = 10.0f;
	if (body->GetLinearVelocity().x < -MAX_SPEED) {
		body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
	}
	else if (body->GetLinearVelocity().x > MAX_SPEED){
		body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
	}
	

	m_onGround = false;

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


				if (manifold.points[i].y < body->GetPosition().y - m_capsule.getDimensions().y / 2.0f + m_capsule.getDimensions().x / 2.0f + 0.01f) {

					below = true;

					// this break will break out of the loop since only needs to be touching once to jump
					break;
				}
			}
			if (below) {

				m_onGround = true;

				// Play can jump
				if (inputManager.isKeyPressed(SDLK_w) || inputManager.isKeyPressed(SDLK_UP)) {
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



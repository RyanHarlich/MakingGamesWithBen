#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}


/* NEW: argement fixedRotation, texture, UV */
void Box::init(b2World* world, 
	const glm::vec2 position, 
	const glm::vec2 dimensions, 
	Bengine::GLTexture texture, 
	Bengine::ColorRGBA8 color, 
	bool fixedRotation, 
	glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/) {

	m_dimensions = dimensions;
	m_color = color;

	/* NEW */
	m_texture = texture;
	m_uvRect = uvRect;

	// Make the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);

	/* NEW */
	bodyDef.fixedRotation = fixedRotation;

	m_body = world->CreateBody(&bodyDef);

	// Make the shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f); // SetAsBox ask for half width and half height, so it is not really setting half the dimensions

	// Make the fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);



}


/* NEW */
void Box::draw(Bengine::SpriteBatch& spriteBatch) {

	glm::vec4 destRect;

	// The position of the box is actually the center of the box not the corner of the box
	destRect.x = m_body->GetPosition().x - (m_dimensions.x / 2.0f);
	destRect.y = m_body->GetPosition().y - (m_dimensions.y / 2.0f);

	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;


	/* NEW: changed to customizable uvRect argument */
	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());

}

#include "Box.h"



Box::Box()
{
}


Box::~Box()
{
}


/* NEW: update added isDynamic and angle */
void Box::init(b2World* world, 
	const glm::vec2 position, 
	const glm::vec2 dimensions, 
	Bengine::GLTexture texture, 
	Bengine::ColorRGBA8 color, 
	bool fixedRotation,
	bool isDynamic,
	float angle, /* = 0.0f */	
	glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/) {

	m_dimensions = dimensions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;

	// Make the body
	b2BodyDef bodyDef;


	/* NEW */
	//bodyDef.type = b2_dynamicBody;
	if (isDynamic) {
		bodyDef.type = b2_dynamicBody;
	}
	else {
		bodyDef.type = b2_staticBody;
	}
	/* NEW: end of new */


	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = fixedRotation;


	/* NEW */
	bodyDef.angle = angle;


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
void Box::destroy(b2World * world) {
	world->DestroyBody(m_body);
}







void Box::draw(Bengine::SpriteBatch& spriteBatch) {

	glm::vec4 destRect;

	// The position of the box is actually the center of the box not the corner of the box
	destRect.x = m_body->GetPosition().x - (m_dimensions.x / 2.0f);
	destRect.y = m_body->GetPosition().y - (m_dimensions.y / 2.0f);

	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;


	spriteBatch.draw(destRect, m_uvRect, m_texture.id, 0.0f, m_color, m_body->GetAngle());

}

#pragma once

#include <Bengine/Vertex.h>
#include <Bengine/GLTexture.h>
#include <Bengine/SpriteBatch.h>

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

class Box
{
public:
	Box();
	~Box();

	/* NEW: update added isDynamic and angle */
	void init(b2World* world, 
		const glm::vec2 position, 
		const glm::vec2 dimensions, 
		Bengine::GLTexture texture, 
		Bengine::ColorRGBA8 color, 
		bool fixedRotation, 
		bool isDynamic,
		float angle = 0.0f,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


	/* NEW */
	void destroy(b2World* world);


	void draw(Bengine::SpriteBatch& spriteBatch);


	/* NEW */
	bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }

	/* NEW */
	// test if a point is inside the box such as clicking
	bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }



	// Getter
	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture() const  { return m_fixture; }
	const glm::vec2& getDimensions() const { return m_dimensions; }


	/* NEW */
	glm::vec2 getPosition() const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }


	const Bengine::ColorRGBA8& getColor() const { return m_color; }

private:
	glm::vec4 m_uvRect;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	Bengine::ColorRGBA8 m_color;
	Bengine::GLTexture m_texture;
};


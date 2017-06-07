#pragma once

#include <Bengine/DebugRenderer.h>

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

class Capsule
{
public:
	void init(b2World* world,
		const glm::vec2 position,
		const glm::vec2 dimensions,
		float density,
		float friction,
		bool fixedRotation);



	void destroy(b2World* world);



	// usually this would go in a seperate render class like the ball game
	void drawDebug(Bengine::DebugRenderer& debugRenderer);

	// Getter
	b2Body* getBody() const { return m_body; }
	b2Fixture* getFixture(int index) const { return m_fixtures[index]; }
	const glm::vec2& getDimensions() const { return m_dimensions; }

private:
	b2Body* m_body = nullptr;
	b2Fixture* m_fixtures[3];
	glm::vec2 m_dimensions;
};


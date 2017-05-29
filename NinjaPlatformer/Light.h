#pragma once

#include <Bengine/SpriteBatch.h>
#include <Bengine/Vertex.h>

#include <glm/glm.hpp>

class Light {
public:
	void draw(Bengine::SpriteBatch& spriteBatch) {
		glm::vec4 destRect;
		// The position of the box is actually the center of the box not the corner of the box
		destRect.x = position.x - (size / 2.0f);
		destRect.y = position.y - (size / 2.0f);
		destRect.z = size;
		destRect.w = size;
		spriteBatch.draw(destRect, glm::vec4(-1, -1, 2, 2.0f), 0, 0.0f, color, 0);
	}

	Bengine::ColorRGBA8 color;
	glm::vec2 position;
	float size;
};

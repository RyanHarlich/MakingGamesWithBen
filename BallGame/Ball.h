#pragma once

#include <Bengine/Vertex.h>

#include <glm/glm.hpp>


struct Cell;


struct Ball {
	Ball(float radiu, float mas, const glm::vec2& pos,
		const glm::vec2& vel, unsigned int texturId,
		const Bengine::ColorRGBA8& colo);

	float radius;
	float mass;
	glm::vec2 velocity;
	glm::vec2 position;
	unsigned int textureID = 0;
	Bengine::ColorRGBA8 color;
	Cell* ownerCell = nullptr; // nullptr violations are alot easier to debug than random violations memory
	int cellVectorIndex = -1;

};


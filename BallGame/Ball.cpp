#include "Ball.h"



Ball::Ball(float radiu, float mas, const glm::vec2& pos,
	const glm::vec2& vel, unsigned int texturId,
	const Bengine::ColorRGBA8& colo) {
	radius = radiu;
	mass = mas;
	position = pos;
	velocity = vel;
	textureID = texturId;
	color = colo;
}




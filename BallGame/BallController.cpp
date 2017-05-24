#include "BallController.h"




void BallController::updateBalls(std::vector <Ball>& balls, float deltaTime, int maxX, int maxY) {
	const float FRICTION = 0.001f;
	// Update our grabbed balls velocity
	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
	}

	glm::vec2 gravity = getGravityAccel();

	for (size_t i = 0; i < balls.size(); ++i) {
		// get handle for less typing
		Ball& ball = balls[i];
		// Update motion if its not grabbed
		if (i != m_grabbedBall) {
			ball.position += ball.velocity * deltaTime;
			// Apply friction
			glm::vec2 momentumVec = ball.velocity * ball.mass;
			if (momentumVec.x != 0 || momentumVec.y != 0) {
				if (FRICTION < glm::length(momentumVec)) {
					ball.velocity -= deltaTime * FRICTION * glm::normalize(momentumVec) / ball.mass;
				}
				else {
					ball.velocity = glm::vec2(0.0f);
				}
			}
			// Apply gravity
			ball.velocity += gravity * deltaTime;
		}
		//Check wall collision
		if (ball.position.x < 0) {
			ball.position.x = 0;
			if (ball.velocity.x < 0) ball.velocity.x *= -1;
		}
		else if (ball.position.x + ball.radius * 2.0f >= maxX) {
			ball.position.x = maxX - ball.radius * 2.0f - 1;
			if (ball.velocity.x > 0) ball.velocity.x *= -1;
		}
		if (ball.position.y < 0) {
			ball.position.y = 0;
			if (ball.velocity.y < 0) ball.velocity.y *= -1;
		}
		else if (ball.position.y + ball.radius * 2.0f >= maxY) {
			ball.position.y = maxY - ball.radius * 2.0f - 1;
			if (ball.velocity.y > 0) ball.velocity.y *= -1;
		}

		//Check collisions
		for (size_t j = i + 1; j < balls.size(); ++j) {
			checkCollision(ball, balls[j]);
		}
	}
	// Update our grabbed ball
	if (m_grabbedBall != -1) {
		// Update the velocity againm in case if got change by collision
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
		m_prevPos = balls[m_grabbedBall].position;
	}

}








void BallController::onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY) {
	for (size_t i = 0; i < balls.size(); ++i) {
		// Check if the mouse is hovering over a ball
		if (isMouseOnBall(balls[i], mouseX, mouseY)) {
			m_grabbedBall = i;
			m_grabOffSet = glm::vec2(mouseX, mouseY) - balls[i].position;
			m_prevPos = balls[i].position;
			balls[i].velocity = glm::vec2(0.0f);
		}
	}
}







void BallController::onMouseUp(std::vector <Ball>& balls) {
	if (m_grabbedBall != -1) {
		// Throw the ball!
		balls[m_grabbedBall].velocity = balls[m_grabbedBall].position - m_prevPos;
		m_grabbedBall = -1;
	}
}







void BallController::onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY) {
	if (m_grabbedBall != -1) {
		balls[m_grabbedBall].position = glm::vec2(mouseX, mouseY) - m_grabOffSet;
	}
}







void BallController::checkCollision(Ball& b1, Ball& b2) {
	// We add radius since position is the top left corner
	glm::vec2 distVec = (b2.position + b2.radius) - (b1.position + b1.radius);
	glm::vec2 distDir = glm::normalize(distVec);
	float dist = glm::length(distVec);
	float totalRadius = b1.radius + b2.radius;

	float collisionDepth = totalRadius - dist;
	// Check for collision
	if (collisionDepth > 0) {

		// Push away the less massive one
		if (b1.mass < b2.mass) {
			b1.position -= distDir * collisionDepth;
		}
		else {
			b2.position += distDir * collisionDepth;
		}

		// Calcuate deflectiopn. http://stackoverflow.com/a/345863
		float aci = glm::dot(b1.velocity, distDir) / b2.mass;
		float bci = glm::dot(b2.velocity, distDir) / b1.mass;

		float massRatio = b1.mass / b2.mass;

		b1.velocity += (bci - aci) * distDir * (1.0f / massRatio);
		b2.velocity += (aci - bci) * distDir * massRatio;

	}
}






bool BallController::isMouseOnBall(Ball& b, float mouseX, float mouseY) {
	return (mouseX >= b.position.x && mouseX < b.position.x + b.radius * 2.0f && mouseY >= b.position.y && mouseY < b.position.y + b.radius * 2.0f);
}





glm::vec2 BallController::getGravityAccel() {
	const float GRAVITY_FORCE = 0.1f;
	glm::vec2 gravity;

	switch (m_gravityDirection) {
	case GravityDirection::DOWN:
		gravity = glm::vec2(0.0f, -GRAVITY_FORCE);
		break;
	case GravityDirection::LEFT:
		gravity = glm::vec2(-GRAVITY_FORCE,0.0f);
		break;
	case GravityDirection::RIGHT:
		gravity = glm::vec2(GRAVITY_FORCE,0.0f);
		break;
	case GravityDirection::UP:
		gravity = glm::vec2(0.0f, GRAVITY_FORCE);
		break;
	}

	return gravity;
}

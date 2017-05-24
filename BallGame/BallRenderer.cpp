#include "BallRenderer.h"



void BallRenderer::renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball) {
	spriteBatch.draw(
		glm::vec4(ball.position.x, ball.position.y, ball.radius * 2, ball.radius * 2),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		ball.textureID,
		0.0f,
		ball.color
	);

}

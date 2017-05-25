#include "BallRenderer.h"



void BallRenderer::renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball) {

	/* NEW: change position vector to subtract radius from x and y direction: to get rectangle to start in the top left hand corner of the ball */
	/*spriteBatch.draw(
		glm::vec4(ball.position.x, ball.position.y, ball.radius * 2, ball.radius * 2),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		ball.textureID,
		0.0f,
		ball.color
	);*/
	spriteBatch.draw(
		glm::vec4(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2, ball.radius * 2),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		ball.textureID,
		0.0f,
		ball.color
	);

}

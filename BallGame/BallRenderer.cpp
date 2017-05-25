#include "BallRenderer.h"



void BallRenderer::renderBalls(Bengine::SpriteBatch & spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix){


	/* NEW: switch back to regular background */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	// Lazily initialize the program
	if (m_program == nullptr) {
		m_program = std::make_unique<Bengine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->use();

	/* NEW: moved here from MainGame.cpp to make sure spriteBatch renders batch before the shader program is unused, so moved all spriteBatch calls here (omit init) */
	spriteBatch.begin();

	// Grab camera matrix
	glUniform1i(m_program->getUniformLocation("mySampler"), 0);
	glUniformMatrix4fv(m_program->getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);

	// Render all the balls
	for (auto& ball : balls) {
		spriteBatch.draw(
			glm::vec4(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2, ball.radius * 2),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			ball.textureID,
			0.0f,
			ball.color);
	}


	/* NEW: moved here from MainGame.cpp to make sure spriteBatch renders batch before the shader program is unused */
	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();


}





/* NEW: momentum is equal to the velocity * the mass */
void MomentumBallRenderer::renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {

	/* NEW: blue background */
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);

	// Lazily initialize the program
	if (m_program == nullptr) {
		m_program = std::make_unique<Bengine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->use();
	spriteBatch.begin();
	// Grab camera matrix
	glUniform1i(m_program->getUniformLocation("mySampler"), 0);
	glUniformMatrix4fv(m_program->getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);


	// Render all the balls
	for (auto& ball : balls) {

		/* NEW: color added to change with respect to its ball's velocity * its mass (which is momentum, momentum = velocity * mass) */
		Bengine::ColorRGBA8 color;
		// needs to be a GLubyte so the colorVal is between 0-255, and clamp needs to be a float
		/* NEW: IMPORTANT: NEED TO MULTIPLY MASS BY 12 OR ELSE THERE WILL NOT BE ENOUGH MOMENTUM */
		GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
		color.r = colorVal;
		color.g = colorVal;
		color.b = colorVal;
		color.a = colorVal;

		spriteBatch.draw(
			glm::vec4(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2, ball.radius * 2),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			ball.textureID,
			0.0f,
			color); /* NEW: changed to the new color */
	}




	spriteBatch.end();
	spriteBatch.renderBatch();

	m_program->unuse();


}






VelocityBallRenderer::VelocityBallRenderer(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight) {
}







/* NEW: only visually velocity in the x direction, visualize position with color */
void VelocityBallRenderer::renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {

	/* NEW: blue background */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Lazily initialize the program
	if (m_program == nullptr) {
		m_program = std::make_unique<Bengine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->use();
	spriteBatch.begin();
	// Grab camera matrix
	glUniform1i(m_program->getUniformLocation("mySampler"), 0);
	glUniformMatrix4fv(m_program->getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);


	// Render all the balls
	for (auto& ball : balls) {

		/* NEW: color added to change with respect to its ball's velocity only in x direction, while visualizing position with color */
		Bengine::ColorRGBA8 color;
		float mult = 100.0f; //multiplier
		// needs to be a GLubyte so the colorVal is between 0-255, and clamp needs to be a float
		GLubyte colorVal = (GLubyte)(glm::clamp(ball.velocity.x * mult, 0.0f, 255.0f));
		color.r = 128;
		color.g = (GLubyte)((ball.position.x / m_screenWidth) * 255.0f); // will give a number between 0-1 so multiply
		color.b = (GLubyte)((ball.position.y / m_screenHeight) * 255.0f); // will give a number between 0-1 so multiply
		color.a = colorVal;

		spriteBatch.draw(
			glm::vec4(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2, ball.radius * 2),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			ball.textureID,
			0.0f,
			color); /* NEW: changed to the new color */
	}




	spriteBatch.end();
	spriteBatch.renderBatch();
	m_program->unuse();
}







/*  NEW: no comments, same as momentum but with black background, my favorite one */
void SelfMade::renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


	if (m_program == nullptr) {
		m_program = std::make_unique<Bengine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->use();
	spriteBatch.begin();

	glUniform1i(m_program->getUniformLocation("mySampler"), 0);
	glUniformMatrix4fv(m_program->getUniformLocation("P"), 1, GL_FALSE, &projectionMatrix[0][0]);



	for (auto& ball : balls) {
		Bengine::ColorRGBA8 color;
		GLubyte colorVal = (GLubyte)(glm::clamp(glm::length(ball.velocity) * ball.mass * 12.0f, 0.0f, 255.0f));
		color.r = colorVal;
		color.g = colorVal;
		color.b = colorVal;
		color.a = colorVal;

		spriteBatch.draw(
			glm::vec4(ball.position.x - ball.radius, ball.position.y - ball.radius, ball.radius * 2, ball.radius * 2),
			glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
			ball.textureID,
			0.0f,
			color); 
	}




	spriteBatch.end();
	spriteBatch.renderBatch();
	m_program->unuse();

}






/* NEW */
TrippyBallRenderer::TrippyBallRenderer(int screenWidth, int screenHeight) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight) {
	// Empty
}







/* NEW: copied and pasted from GitHub, not in video */
void TrippyBallRenderer::renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) {
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);

	// Lazily initialize the program
	if (m_program == nullptr) {
		m_program = std::make_unique<Bengine::GLSLProgram>();
		m_program->compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
		m_program->addAttribute("vertexPosition");
		m_program->addAttribute("vertexColor");
		m_program->addAttribute("vertexUV");
		m_program->linkShaders();
	}
	m_program->use();
	spriteBatch.begin();
	// Make sure the shader uses texture 0
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = m_program->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	// Grab the camera matrix
	GLint pUniform = m_program->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);





	// Change these constants to get cool stuff
	float TIME_SPEED = 0.01f;
	float DIVISOR = 4.0f; // Increase to get more arms
	float SPIRAL_INTENSITY = 10.0f; // Increase to make it spiral more

	m_time += TIME_SPEED;



	// Render all the balls
	for (auto& ball : balls) {

		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		const glm::vec4 destRect(ball.position.x - ball.radius, ball.position.y - ball.radius,
			ball.radius * 2.0f, ball.radius * 2.0f);
		Bengine::ColorRGBA8 color;
		// Get vector from center point
		glm::vec2 centerVec = ball.position - glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
		float centerDist = glm::length(centerVec);
		// Get angle from the horizontal
		float angle = atan2(centerVec.x, centerVec.y) / (3.1415926f / DIVISOR);
		// Move with time
		angle -= m_time;
		// Add the spiral
		angle += (centerDist / m_screenWidth) * SPIRAL_INTENSITY;

		color.r = (GLubyte)(angle * 255.0f);
		color.g = (GLubyte)(angle * 255.0f * cos(m_time));
		color.b = (GLubyte)(angle * 255.0f * sin(m_time));
		color.a = (GLubyte)(glm::clamp(1.0f - (centerDist / (m_screenWidth / 2.0f)), 0.0f, 1.0f) * 255.0f);
		spriteBatch.draw(destRect, uvRect, ball.textureID, 0.0f, color);
	}





	spriteBatch.end();
	spriteBatch.renderBatch();
	m_program->unuse();
}
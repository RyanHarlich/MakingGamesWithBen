#pragma once

#include <vector>
#include <memory>

#include "Ball.h"

#include <Bengine/SpriteBatch.h>
#include <Bengine/GLSLProgram.h>


class BallRenderer
{
public:
	virtual void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);
protected:
	std::unique_ptr<Bengine::GLSLProgram> m_program = nullptr;

};

// momentum = velocity * mass
class MomentumBallRenderer : public BallRenderer {
public:
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
};


// only visually velocity in the x direction, visualize position with color 
class VelocityBallRenderer : public BallRenderer {
public:
	VelocityBallRenderer(int screenWidth, int screenHeight);
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
private:
	int m_screenWidth;
	int m_screenHeight;
};



class SelfMade : public BallRenderer {
public:
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
};



class TrippyBallRenderer : public BallRenderer {
public:
	TrippyBallRenderer(int screenWidth, int screenHeight);
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
private:
	int m_screenWidth;
	int m_screenHeight;
	float m_time;
};














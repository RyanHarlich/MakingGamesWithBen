#pragma once

/* NEW */
#include <vector>
#include <memory>

#include "Ball.h"

#include <Bengine/SpriteBatch.h>

/* NEW */
#include <Bengine/GLSLProgram.h>


class BallRenderer
{
public:
	/* NEW: removed old render method and replaced with the one below */
	//void renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball);
	virtual void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix);

	/* NEW */
protected:
	/* NEW: for this to be properly destoryed, need to make sure there is a destructor in the ballrenderer class */
	std::unique_ptr<Bengine::GLSLProgram> m_program = nullptr;


};


/* NEW: now can add a bunch of different types of renders! */

/* NEW: new ball render class */
class MomentumBallRenderer : public BallRenderer {
public:
	/* NEW NEEDS TO INHERIENT VIRTUAL VOID FUNCTION because the inheriented class is abstract */
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override; /* NEW: C++ 11 feature that prevents bugs in case changing signature of base class virtual function, also states that the virtual function is being overridden */
};


/* NEW: now can add a bunch of different types of renders! */

/* NEW: new ball render class */
class VelocityBallRenderer : public BallRenderer {
public:
	VelocityBallRenderer(int screenWidth, int screenHeight);
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;

private:
	int m_screenWidth;
	int m_screenHeight;
};






/*  NEW: no comments, same as momentum but with black background, my favorite one */
class SelfMade : public BallRenderer {
public:
	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;
};





/* NEW: copied and pasted from GitHub, not in video */
class TrippyBallRenderer : public BallRenderer {
public:
	TrippyBallRenderer(int screenWidth, int screenHeight);

	void renderBalls(Bengine::SpriteBatch& spriteBatch, const std::vector<Ball>& balls, const glm::mat4& projectionMatrix) override;

private:
	int m_screenWidth;
	int m_screenHeight;
	float m_time;
};














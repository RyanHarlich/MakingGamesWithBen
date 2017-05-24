#pragma once

#include <Bengine/SpriteBatch.h>
#include "Ball.h"

class BallRenderer
{
public:
	void renderBall(Bengine::SpriteBatch& spriteBatch, Ball& ball);
};


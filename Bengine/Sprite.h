#pragma once

#include <string>

#include "GLTexture.h"

#include <GL/glew.h>



namespace Bengine {

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		float _x;
		float _y;
		float _width;
		float _height;
		GLuint _vboID;
		GLTexture _texture;
	};

}




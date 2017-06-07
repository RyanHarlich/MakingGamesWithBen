#pragma once

#include <string>

#include <GL/glew.h>


namespace Bengine {

	struct GLTexture {
		std::string filePath = "";
		GLuint id;
		int height;
		int width;
	};

}
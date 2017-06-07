#pragma once

/* NEW */
#include <string>

#include <GL/glew.h>


namespace Bengine {

	struct GLTexture {

		/* NEW */
		std::string filePath = "";

		GLuint id;
		int height;
		int width;
	};

}
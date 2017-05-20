#pragma once

#include <string>

#include "GLTexture.h"


namespace Bengine {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}

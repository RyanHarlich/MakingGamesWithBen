#pragma once

#include "GLTexture.h"

#include <glm/glm.hpp>

namespace Bengine {

	class TileSheet {
	public:
		void init(const GLTexture& texture, glm::ivec2& tileDims) {
			this->texture = texture;
			this->dims = tileDims;
		}

		glm::vec4 getUVs(int index) {
			int xTile = index % dims.x;
			int yTile = index / dims.x;

			glm::vec4 uvs;
			uvs.x = xTile / (float)dims.x; // the place of the x sheet to start at
			uvs.y = yTile / (float)dims.y; // the place of the y sheet to start at
			uvs.z = 1.0f / dims.x; // what percentage of the x sheet 
			uvs.w = 1.0f / dims.y; // what percentage of the y sheet 

			return uvs;
		}

		GLTexture texture;
		glm::ivec2 dims;


	};

}
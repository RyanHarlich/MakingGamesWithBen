#include <iostream>

#include "TextureCache.h"
#include "ImageLoader.h"




namespace Bengine {

	TextureCache::TextureCache()
	{
	}




	TextureCache::~TextureCache()
	{
	}





	GLTexture TextureCache::getTexture(std::string texturePath) {

		auto mit = m_textureMap.find(texturePath);

		if (mit == m_textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			m_textureMap.insert(make_pair(texturePath, newTexture));

			// commented out, will assume texture was loaded and there is no errors
			//std::cout << "Loaded Texture!\n";
			return newTexture;
		}


		// commented out, will assume texture was used and there is no errors
		//std::cout << "Used Cached Texture!\n";

		return mit->second;
	}

}
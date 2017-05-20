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

		auto mit = _textureMap.find(texturePath);

		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			_textureMap.insert(make_pair(texturePath, newTexture));

			// commented out, will assume texture was loaded and there is no errors
			//std::cout << "Loaded Texture!\n";
			return newTexture;
		}


		// commented out, will assume texture was used and there is no errors
		//std::cout << "Used Cached Texture!\n";

		return mit->second;
	}

}
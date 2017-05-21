#include <iostream>

#include "BengineErrors.h"

#include <SDL/SDL.h>


namespace Bengine {

	void fatalError(std::string errorMessage) {
		std::cout << errorMessage << std::endl;
		std::cout << "Press any key to quit... ";
		char tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(1);
	}

}
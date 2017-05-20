#pragma once

#include <string>

#include <SDL/SDL.h>
#include <GL/glew.h>



namespace Bengine {

	// cannot be an enum class since it is going to be used for bitwise operations
	// 0, 10, 100 would be as shown for the 1st bit (0x1), the 2nnd bit (0x2), and the 3rd bit (0x4)
	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		//getters
		int getScreenWidth() { return _screenWidth; }
		int getScreenHeight() { return _screenHeight; }


	private:
		SDL_Window* _sdlWindow;
		int _screenWidth, _screenHeight;

	};

}
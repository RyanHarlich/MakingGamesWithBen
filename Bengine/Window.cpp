#include "Window.h"
#include "Errors.h"


namespace Bengine {

	Window::Window()
	{
	}




	Window::~Window()
	{
	}





	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}



		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
		if (_sdlWindow == nullptr) {
			fatalError("The window was not created!");
		}


		SDL_GLContext context = SDL_GL_CreateContext(_sdlWindow);
		if (context == nullptr) {
			fatalError("The GL Context was not created!");
		}


		GLenum error = glewInit();
		if (error != GLEW_OK) {
			perror("GLEW was not initiated!");
		}


		std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		SDL_GL_SetSwapInterval(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}





	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}

}

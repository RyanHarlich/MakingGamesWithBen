#pragma once

#include <string>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <SDL/SDL_events.h> // could include SDL.h but to improve compile time only include what is needed


namespace Bengine {

	class GUI
	{
	public:


		void init(const std::string& resourceDirectory);
		void destroy();

		void draw();
		void update();


		void setMouseCursor(const std::string& imageFile);
		void showMouseCursor();
		void hideMouseCursor();

		void onSDLEvent(SDL_Event& evnt);


		void loadScheme(const std::string& schemeFile);

		void setFont(const std::string& fontFile);

		//Widgets are all windows, they all inherit from the Windows class
		// look at the schemas that is choosen/loaded from the GUI directory for information such as type casting (can also change the schema)
		// scheme is actually called type here or it is refering to button of the scheme
		CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = ""); // good idea to give it a name in case want to look it up later, but do not have to

		static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

		//Getters
		static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; } // this is kind of like a global variable since it is static
		const CEGUI::GUIContext* getContext() const { return m_context; }

	private:
		static CEGUI::OpenGL3Renderer* m_renderer; // cannot do a header initialization because it is static
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_root = nullptr;
		unsigned int m_lastTime = 0;


	};

}
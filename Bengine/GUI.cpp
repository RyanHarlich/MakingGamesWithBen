#include "GUI.h"


namespace Bengine {

	CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;




	void GUI::init(const std::string& resourceDirectory) {
		// Check if the renderer and system were not already initialized
		if (m_renderer == nullptr) {

			// This initializes all of CEGUI subsystems and initializes the renderer
			m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem(); // returns a reference not a pointer, so need the address of the reference

			// Loads resources that are provided from the GUI directory
			CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider()); // this gets a global system object // with a safe type cast

			// Set up a resource manager for each resource group that the GUI directory contains
			rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
			rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
			rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
			rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
			rp->setResourceGroupDirectory("looknfeels", resourceDirectory + "/looknfeel/");
			rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

			// Does not need to be imagesets, but does need to be the same name above that was given for the image resource group
			CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
			// notice the following have a different function then the above
			CEGUI::Scheme::setDefaultResourceGroup("schemes");
			CEGUI::Font::setDefaultResourceGroup("fonts");		
			CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
			CEGUI::WindowManager::setDefaultResourceGroup("layouts");
			CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

		}

		// Create GUI context
		m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());

		//Create a view to add widgets to, so need a default root window
		m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
		m_context->setRootWindow(m_root);
	}





	void GUI::destroy() {
		CEGUI::System::getSingleton().destroyGUIContext(*m_context);
	}





	void GUI::draw() {
		// Can call a function that renders all GUI (somthing like CGI render all GUI context), instead only want to render one GUI at a time so this is a better function to call
		m_renderer->beginRendering();

		m_context->draw();

		m_renderer->endRendering();

		// TODO: BUG HERE, although do not have this bug for some reason, have the same bug but need to enable the alpha blend below with glEnable(GL_BLEND)
		// Discards pixels outside of the rectangle, discards  pixels outside of the rectangle for the button
		glDisable(GL_SCISSOR_TEST);

		// Another bug, not sure why because the tutorial did not go over this
		glEnable(GL_BLEND);
	}





	void GUI::loadScheme(const std::string & schemeFile) {
		CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
	}





	void GUI::setFont(const std::string & fontFile) {

		// load font
		CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
		// set font
		m_context->setDefaultFont(fontFile);
	}





	// Do not redeclare default parameters
	CEGUI::Window * GUI::createWidget(const std::string & type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string & name /* = ""*/) {

		// Create the new Widget
		CEGUI::Window * newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
		// Add new widget to root window
		m_root->addChild(newWindow);

		setWidgetDestRect(newWindow, destRectPerc, destRectPix);

		// Return new Window widget
		return newWindow;

		// scheme is actually called type here or it is refering to button of the scheme
		//return CEGUI::WindowManager::getSingleton().createWindow(type, name);
	}




	// Sets as a percentage of the root window rather than actual dimensions
	void GUI::setWidgetDestRect(CEGUI::Window * widget, const glm::vec4 & destRectPerc, const glm::vec4 & destRectPix) {
		widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), CEGUI::UDim(destRectPerc.y, destRectPix.y)));
		widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPerc.z, destRectPix.z), CEGUI::UDim(destRectPerc.w, destRectPix.w))); // where z is width and w is height
	}





}

#include "App.h"

#include <blib/gl/Window.h>

namespace blib
{

	void App::run()
	{
		window = new blib::gl::Window();
		window->setSize(appSetup.width, appSetup.height);
		window->setBorder(appSetup.border);
		window->create();

		for(int i = 0; i < 255; i++)
			keyState.pressedKeys[i] = false;
		class AppKeyListener : public KeyListener {
			App* app;
		public:
			AppKeyListener(App* app)			{			this->app = app;							}
			void onKeyDown( blib::Key key )		{			app->keyState.pressedKeys[key] = true;		}
			void onKeyUp( blib::Key key )		{			app->keyState.pressedKeys[key] = false;		}
		} listener(this);
		addListener(&listener);



		init();
		running = true;
		while(running)
		{
			window->tick();
			update();
			draw();
			window->swapBuffers();
		}
	}



	void App::addListener(KeyListener* keyListener)
	{
		window->addListener(keyListener);
	}
	void App::addListener(MouseListener* mouseListener)
	{
		window->addListener(mouseListener);
	}
}
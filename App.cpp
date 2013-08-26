#include "App.h"

#include <blib/gl/Window.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Renderer.h>
#include <blib/util/Profiler.h>
#include <blib/RenderState.h>
#include <blib/MouseListener.h>

namespace blib
{

	void App::start(bool looping)
	{
		window = new blib::gl::Window();
		window->setSize(appSetup.width, appSetup.height);
		window->setBorder(appSetup.border);
		window->create();

		{
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
		}

		{
			class AppMouseListener : public MouseListener {
				App* app;
			public:
				AppMouseListener(App* app)						{	this->app = app;								}
				void onMouseDown(int x, int y, Button button)	{	app->mouseState.buttons[button] = true;	};
				void onMouseUp(int x, int y, Button button)		{	app->mouseState.buttons[button] = true;	};
				void onMouseMove(int x, int y, Buttons button)	{	app->mouseState.x = x; app->mouseState.y = y;};
			} listener(this);
			addListener(&listener);
		}

		renderer = new gl::Renderer();
		renderState = RenderState::activeRenderState;

		blib::gl::GlInitRegister::initRegisteredObjects();

		init();

		blib::gl::GlResizeRegister::ResizeRegisteredObjects(window->getWidth(), window->getHeight());


		running = true;

		if(looping)
			run();
	}

	void App::run()
	{
		while(running)
		{
			step();
		}
	}

	void App::step()
	{
		double elapsedTime = blib::util::Profiler::getTime();
		blib::util::Profiler::startFrame();
		blib::util::Profiler::startSection("frame");
		window->tick();
		time += elapsedTime;
		update(elapsedTime);
		draw();
		window->swapBuffers();
		blib::util::Profiler::endSection("frame");
		Sleep(0);
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
#include "App.h"

#include <blib/gl/Window.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Renderer.h>
#include <blib/util/Profiler.h>
#include <blib/RenderState.h>
#include <blib/MouseListener.h>
#include <blib/SpriteBatch.h>
#include <blib/util/Signal.h>
#include <blib/util/Log.h>

using blib::util::Log;

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

		mouseState.leftButton = false;
		mouseState.rightButton = false;
		mouseState.middleButton = false;
		mouseState.x = 0;
		mouseState.y = 0;

		renderer = new gl::Renderer();
		renderState = RenderState::activeRenderState;
		spriteBatch = new SpriteBatch(renderer);


		blib::gl::GlInitRegister::initRegisteredObjects();

		init();

		blib::gl::GlResizeRegister::ResizeRegisteredObjects(window->getWidth(), window->getHeight());


		running = true;

		renderThread = new RenderThread(this);
		renderThread->start();

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
		
		wglMakeCurrent(NULL, NULL);
		renderThread->renderSignal->signal();
		renderThread->updateSignal->wait();


		if(!wglMakeCurrent(window->hdc, ((blib::gl::Window*)window)->hrc))
		{
			Log::out<<"ERROR MAKING CURRENT"<<Log::newline;
			char* lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
			Log::out<<"Error: "<<lpMsgBuf<<Log::newline;
		}


		//renderer->flush();

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










	App::RenderThread::RenderThread( App* app ) : Thread("RenderThread")
	{
		this->app = app;
		renderSignal = new blib::util::Signal();
		updateSignal = new blib::util::Signal();
	}


	int App::RenderThread::run()
	{
		while(true)
		{
			renderSignal->wait();
			if(!wglMakeCurrent(app->window->hdc, ((blib::gl::Window*)app->window)->hrc))
			{
				Log::out<<"ERROR MAKING CURRENT"<<Log::newline;
				char* lpMsgBuf;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
				Log::out<<"Error: "<<lpMsgBuf<<Log::newline;

			}
			app->renderer->flush();
			app->window->swapBuffers();
			
			wglMakeCurrent(NULL, NULL);

			updateSignal->signal();
		}
	}

}
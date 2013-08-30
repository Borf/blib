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
#include <blib/util/Mutex.h>
#include <blib/util/Log.h>
#include <blib/util/Semaphore.h>

using blib::util::Log;

namespace blib
{

	void App::start(bool looping)
	{
		semaphore = new util::Semaphore(0,2);

		updateThread = new UpdateThread(this);
		updateThread->start();
		updateThread->semaphore->wait(); //wait until it is initialized


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


	void App::addKeyListener(KeyListener* keyListener)
	{
		window->addListener(keyListener);
	}
	void App::addMouseListener(MouseListener* mouseListener)
	{
		window->addListener(mouseListener);
	}


	void App::createWindow()
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
			};
			addKeyListener(new AppKeyListener(this));
		}

		{
			class AppMouseListener : public MouseListener {
				App* app;
			public:
				AppMouseListener(App* app)						{	this->app = app;								}
				void onMouseDown(int x, int y, Button button)	{	app->mouseState.buttons[button] = true;	};
				void onMouseUp(int x, int y, Button button)		{	app->mouseState.buttons[button] = true;	};
				void onMouseMove(int x, int y, Buttons button)	{	app->mouseState.x = x; app->mouseState.y = y;};
			};
			addMouseListener(new AppMouseListener(this));
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
		wglMakeCurrent(NULL, NULL);

		running = true;
	}


	App::RenderThread::RenderThread( App* app ) : Thread("RenderThread")
	{
		this->app = app;
		semaphore = new blib::util::Semaphore(0,1);
	}
	App::UpdateThread::UpdateThread( App* app ) : Thread("UpdateThread")
	{
		this->app = app;
		semaphore = new blib::util::Semaphore(0,1);
	}


	void App::step()
	{
		renderer->swap();
		renderThread->semaphore->signal();
		updateThread->semaphore->signal();
		semaphore->wait();
		semaphore->wait();

	}



	int App::RenderThread::run()
	{
		if(!wglMakeCurrent(app->window->hdc, ((blib::gl::Window*)app->window)->hrc))
		{
			Log::out<<"ERROR MAKING CURRENT"<<Log::newline;
			char* lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
			Log::out<<"Error: "<<lpMsgBuf<<Log::newline;
		}
		while(app->running)
		{
			semaphore->wait();
			if(!app->running)
				break;
			//Log::out<<"Render"<<Log::newline;
			app->renderer->flush();
			app->window->swapBuffers();
			//wglMakeCurrent(NULL, NULL);
			//signal->signal();
			app->semaphore->signal();
		}
		app->semaphore->signal();
		return 0;
	}

	int App::UpdateThread::run()
	{
		app->createWindow();

		semaphore->signal();

		while(app->running)
		{
			semaphore->wait();
			if(!app->running)
				break;
			//Log::out<<"Update"<<Log::newline;
			app->window->tick();

			double elapsedTime = blib::util::Profiler::getTime();
			blib::util::Profiler::startFrame();
	//		blib::util::Profiler::startSection("frame");
			app->time += elapsedTime;
			app->update(elapsedTime);
			if(!app->running)
				break;
			app->draw();
			app->semaphore->signal();
		}
		app->semaphore->signal();
		return 0;
	}


}
#ifndef WIN32
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#include "App.h"
#include <glm/gtc/matrix_transform.hpp>
#include <blib/gl/Window.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/util/Profiler.h>
#include <blib/RenderState.h>
#include <blib/MouseListener.h>
#include <blib/SpriteBatch.h>
#include <blib/Renderer.h>
#include <blib/util/Signal.h>
#include <blib/util/Mutex.h>
#include <blib/util/Log.h>
#include <blib/util/Semaphore.h>
#include <blib/util/Profiler.h>
#include <blib/SpriteBatch.h>
#include <blib/LineBatch.h>
#include <blib/Box2DDebug.h>
#include <blib/Font.h>
#include <blib/Util.h>
#include <blib/gl/ResourceManager.h>
#ifdef WIN32
#include <gl/wglew.h>
#endif
using blib::util::Log;

namespace blib
{
	App::App()
	{
		time = 0;
	}

	void App::start(bool looping)
	{
		util::Profiler();

		if(appSetup.renderer == AppSetup::NullRenderer)
			resourceManager = new NullResource();
		else if (appSetup.renderer == AppSetup::GlRenderer)
			resourceManager = new gl::ResourceManager();
		else if(appSetup.renderer == AppSetup::DxRenderer)
			resourceManager = new gl::ResourceManager();


		semaphore = new util::Semaphore(0,2);
		updateThread = new UpdateThread(this);	//will create the window in the right thread
		updateThread->start();
		updateThread->semaphore->wait(); //wait until it is initialized
		renderThread = new RenderThread(this);
		renderThread->start();
		joystickThread = new JoystickThread(this);
		joystickThread->start();


		blib::Box2DDebug::getInstance()->init(lineBatch, renderer);

		if(looping)
			run();
	}

	void App::run()
	{
		while(running)
		{
			step();
		}
		joystickThread->waitForTermination();

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
		window->create(appSetup.icon, appSetup.title);

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


#ifdef WIN32
		wglSwapIntervalEXT(appSetup.vsync ? 1 : 0);
#else

#endif


		renderer = resourceManager->getResource<Renderer>();
		renderState = RenderState::activeRenderState;
		spriteBatch = new SpriteBatch(renderer, resourceManager);
		lineBatch = new LineBatch(renderer, resourceManager);

		init();

		blib::gl::GlResizeRegister::ResizeRegisteredObjects(window->getWidth(), window->getHeight());
		frameTimeIndex = 0;
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
	App::JoystickThread::JoystickThread( App* app) : Thread("JoystickThread")
	{
		this->app = app;
	}


	void App::step()
	{
		renderer->swap();
		renderThread->semaphore->signal();
		updateThread->semaphore->signal();
		semaphore->wait();
		semaphore->wait();

		frameTimes[frameTimeIndex].drawTime = renderThread->frameTime;
		frameTimes[frameTimeIndex].updateTime = updateThread->frameTime;
		frameTimes[frameTimeIndex].fps = util::Profiler::fps;
		frameTimeIndex = (frameTimeIndex+1)%1000;
	}



	int App::RenderThread::run()
	{
#ifdef WIN32
		if(!wglMakeCurrent(app->window->hdc, ((blib::gl::Window*)app->window)->hrc))
		{
			Log::out<<"ERROR MAKING CURRENT"<<Log::newline;
			char* lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
			Log::out<<"Error: "<<lpMsgBuf<<Log::newline;
		}
#else


#endif
		while(app->running)
		{
			semaphore->wait();
			if(!app->running)
				break;
			double frameStart = util::Profiler::getAppTime();
			app->renderer->flush();
			app->window->swapBuffers();
			frameTime = util::Profiler::getAppTime() - frameStart;
			app->semaphore->signal();
		}
		app->semaphore->signal();
		return 0;
	}

	int App::UpdateThread::run()
	{
		app->createWindow();

		Texture* gear = app->resourceManager->getResource<Texture>("assets/textures/gear.png");
		Texture* white = app->resourceManager->getResource<Texture>("assets/textures/whitepixel.png");
		Font* font = app->resourceManager->getResource<Font>("tahoma");
#ifdef WIN32
		wglMakeCurrent(NULL, NULL);
#else

#endif

		semaphore->signal();
		while(app->running)
		{
			semaphore->wait();
			if(!app->running)
				break;
			double frameStart = util::Profiler::getAppTime();
			app->window->tick();
			double elapsedTime = blib::util::Profiler::getTime();
			blib::util::Profiler::startFrame();
			app->time += elapsedTime;

			app->update(elapsedTime);
			if(!app->running)
				break;
			app->draw();
			int frame = ((int)(blib::util::Profiler::getAppTime()*50)) % (12*12);

			app->spriteBatch->begin();
			app->spriteBatch->draw(white, glm::scale(glm::translate(glm::mat4(), glm::vec3(20,20,0)), glm::vec3(200.0f,100.0f,1.0f)), glm::vec2(0,0), blib::math::Rectangle(0,0,1,1), glm::vec4(0,0,0,1));

			app->spriteBatch->draw(gear, glm::translate(glm::mat4(), glm::vec3(app->window->getWidth()-80, app->window->getHeight()-80,0)), glm::vec2(0,0), blib::math::Rectangle(1/12.0f * (frame%12),1/12.0f * (frame/12),1/12.0f,1/12.0f));
			app->spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::translate(glm::mat4(), glm::vec3(1,1,0)), glm::vec4(0,0,0,1));
			app->spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::mat4());
			app->spriteBatch->draw(font, "draw time",	glm::scale(glm::translate(glm::mat4(), glm::vec3(20, 20,0)), glm::vec3(0.8f, 0.8f, 1)), glm::vec4(1,0,0,1));
			app->spriteBatch->draw(font, "update time", glm::scale(glm::translate(glm::mat4(), glm::vec3(20, 30,0)), glm::vec3(0.8f, 0.8f, 1)), glm::vec4(0,1,0,1));

			app->spriteBatch->end();
			app->lineBatch->begin();
			app->lineBatch->draw(math::Rectangle(glm::vec2(20,20), 200,100), glm::vec4(1,1,1,1));

			PerformanceInfo minValues = { 99999999999,  99999999999,  99999999999 };
			PerformanceInfo maxValues = { -99999999999, -99999999999, -99999999999 };
			for(int i = 0; i < 1000; i++)
			{
				for(int ii = 0; ii < 3; ii++)
				{
					minValues.data[ii] = glm::min(minValues.data[ii], app->frameTimes[i].data[ii]);
					maxValues.data[ii] = glm::max(maxValues.data[ii], app->frameTimes[i].data[ii]);
				}
			}

			float timeFactor = 100 / (float)glm::max(maxValues.updateTime, maxValues.drawTime);
			PerformanceInfo prevAccum = { 0, 0, 0 };
			PerformanceInfo accum = { 0, 0, 0 };
			for(int i = 0; i < 1000; i++)
			{
				for(int ii = 0; ii < 3; ii++)
					accum.data[ii] += app->frameTimes[i].data[ii];
				if(i%5 == 0 && i > 0)
				{
					for(int ii = 0; ii < 3; ii++)
						accum.data[ii] /= 5.0;

					if(i != 5)
					{
						app->lineBatch->draw(glm::vec2(19 + i*.2f, 120 - timeFactor*prevAccum.drawTime), glm::vec2(20 + i*.2f, 120 - timeFactor*accum.drawTime), glm::vec4(1,0,0,1));
						app->lineBatch->draw(glm::vec2(19 + i*.2f, 120 - timeFactor*prevAccum.updateTime), glm::vec2(20 + i*.2f, 120 - timeFactor*accum.updateTime), glm::vec4(0,1,0,1));
					}
					prevAccum = accum;
					memset(&accum, 0, sizeof(PerformanceInfo));
				}
			}

			app->lineBatch->end();

			frameTime = util::Profiler::getAppTime() - frameStart;
			app->semaphore->signal();
		}
		app->semaphore->signal();
		return 0;
	}



	int App::JoystickThread::run()
	{
		while(app->running)
		{
			int joystickCount = joyGetNumDevs();
			for(int i = 0; i < joystickCount; i++)
			{
				JOYINFOEX info;
				MMRESULT res = joyGetPosEx(i, &info);
				//				if(res == JOYERR_UNPLUGGED)
				//					Log::out<<"Unplugged joystick "<<i<<Log::newline;

				App::JoyState newState;

				if(res != JOYERR_NOERROR)
				{
					newState.connected = false;
					newState.button = 0;
					app->joyStates[i] = newState;
					continue;
				}
				newState.connected = true;
				newState.leftStick.x = 2*(info.dwXpos/65535.0f)-1;
				newState.leftStick.y = 2*(info.dwYpos/65535.0f)-1;

				float len = glm::length(newState.leftStick);
				float angle = atan2(newState.leftStick.y, newState.leftStick.x);

				if(len >= 0.25f)
					newState.leftStick = glm::vec2((len - 0.25f) * (1.0f / (1-0.25f)) * cos(angle), (len - 0.25f) * (1.0f / (1-0.25f)) * sin(angle));
				else
					newState.leftStick = glm::vec2(0,0);

				newState.leftTrigger = glm::max(0.0f, 2*info.dwZpos/65535.0f-1);
				newState.rightTrigger = glm::max(0.0f, 1-2*info.dwZpos/65535.0f);

				newState.rightStick.x = 2*(info.dwUpos/65535.0f)-1;
				newState.rightStick.y = 2*(info.dwVpos/65535.0f)-1;

				newState.button = info.dwButtons;

				//TODO: this is kind of thread unsafe...
				app->joyStates[i] = newState;
			}
			Sleep(0);
		}

		return 0;
	}

}
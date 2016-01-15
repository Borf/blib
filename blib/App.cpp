#ifndef WIN32
#pragma GCC diagnostic ignored "-Wmissing-braces"
#else
#include <gl/glew.h> // for vsync
#endif

#include "App.h"
#include <glm/gtc/matrix_transform.hpp>
#include <blib/gl/Window.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/util/Profiler.h>
#include <blib/RenderState.h>
#include <blib/MouseListener.h>
#include <blib/KeyListener.h>
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
#include <blib/config.h>
#include <blib/Color.h>
#include <blib/ResourceManager.h>
#include <blib/BackgroundTask.h>

//platform specific...kinda
#ifdef BLIB_OPENGL
#include <blib/gl/ResourceManager.h>
#endif


#include <blib/drivers/joystick/Driver.h>
#ifdef BLIB_WINMM
#include <blib/drivers/joystick/WinMM.h>
#endif
#ifdef BLIB_DIRECTINPUT
#include <blib/drivers/joystick/DirectInput.h>
#endif

#ifdef BLIB_WIN
#include <gl/wglew.h>
#endif

#ifdef BLIB_EMSCRIPTEN
#include <emscripten.h>
#endif

using blib::util::Log;

namespace blib
{
	static Texture* gear = NULL;
	static Texture* white = NULL;
	static Font* font = NULL;

	int App::profilerCustomProps = 0;

	App::App()
	{
		time = 0;
#ifdef _DEBUG
		showProfiler = true;
#else
		showProfiler = false;
#endif
		profilerCustomProps = 0;
		joystickDriver = NULL;
		window = NULL;
		resourceManager = NULL;
		renderer = NULL;
		spriteBatch = NULL;
		lineBatch = NULL;
		semaphore = NULL;
		renderThread = NULL;
		updateThread = NULL;
		runnerMutex = NULL;

		for (int i = 0; i < 1000; i++)
		{
			frameTimes[i].drawTime = 0;
			frameTimes[i].fps = 0;
			frameTimes[i].updateTime = 0;
		}

	}

	App::~App()
	{
		if (appSetup.threaded)
		{
		//	renderThread->semaphore->signal();
		//	updateThread->semaphore->signal();
	//		Sleep(40);
			if (!window->makeCurrent())
				return;
		}
		if (joystickDriver)
			delete joystickDriver;
		joystickDriver = NULL;

//		delete resourceManager;
		delete spriteBatch;
		delete lineBatch;
		delete renderer;

		resourceManager->dispose(gear);
		resourceManager->dispose(font);
		resourceManager->dispose(white);


		if (appSetup.threaded)
		{
			renderThread->semaphore->signal();
			renderThread->semaphore->signal();
			renderThread->waitForTermination();
			updateThread->semaphore->signal();
			updateThread->semaphore->signal();
			updateThread->waitForTermination();

			delete semaphore;
			delete renderThread;
			delete updateThread;
		}
		if (appSetup.threaded || appSetup.backgroundTasks)
			delete runnerMutex;
		delete window;

	}

#ifdef BLIB_EMSCRIPTEN
	App* app;
	void blaStep()
	{
		app->step();
	}
#endif


	void App::start(bool looping)
	{
		Log::out<<"App::start"<<Log::newline;
		util::Profiler();

#ifdef BLIB_LINUX
		appSetup.threaded = false;
#endif


		if(appSetup.renderer == AppSetup::NullRenderer)
			resourceManager = new NullResource();
#ifdef BLIB_OPENGL
		else if (appSetup.renderer == AppSetup::GlRenderer)
			resourceManager = new gl::ResourceManager();
		else if (appSetup.renderer == AppSetup::DirectGlRenderer)
			resourceManager = new gl::ResourceManager();
#endif
#ifdef BLIB_DIRECTX
		else if(appSetup.renderer == AppSetup::DxRenderer)
			resourceManager = new gl::ResourceManager();
#endif
		else
			Log::out<<"Invalid renderer"<<Log::newline;


		if(!resourceManager || appSetup.renderer == AppSetup::NullRenderer)
		{
			Log::out<<"No resource manager....quitting";
			exit(0);
			return;
		}

		if (appSetup.threaded || appSetup.backgroundTasks)
			runnerMutex = new util::Mutex();

		if (appSetup.threaded)
		{
			semaphore = new util::Semaphore(0,2);
			updateThread = new UpdateThread(this);	//will create the window in the right thread
			updateThread->start();
			updateThread->semaphore->wait(); //wait until it is initialized

			renderThread = new RenderThread(this);
			renderThread->start();
		}
		else
		{
			createWindow();
			window->makeCurrent();

		}

		if(appSetup.joystickDriver == AppSetup::NullJoystick)
			joystickDriver = NULL;
#ifdef BLIB_WINMM
		else if(appSetup.joystickDriver == AppSetup::WinMM)
			joystickDriver = new drivers::joystick::WinMM();
#endif
#ifdef BLIB_DIRECTINPUT
		else if(appSetup.joystickDriver == AppSetup::DirectInput)
			joystickDriver = new drivers::joystick::DirectInput(window);
#endif
		else
			Log::out<<"Invalid joystick driver"<<Log::newline;
		blib::Box2DDebug::getInstance()->init(lineBatch, renderer);

		blib::util::Profiler::startFrame();
		if (looping)
			run();

	}

	void App::run()
	{
		Log::out<<"App::run"<<Log::newline;
#ifdef BLIB_EMSCRIPTEN
		app = this;
		emscripten_set_main_loop(blaStep, 0, 1);
#else
		while(running)
		{
			step();
		}
#endif
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
		Log::out<<"App::createWindow"<<Log::newline;
		window = new blib::gl::Window(this);
		window->setSize((int)appSetup.window.width(), (int)appSetup.window.height());
		window->moveTo((int)appSetup.window.topleft.x, (int)appSetup.window.topleft.y);
		window->setBorder(appSetup.border);
		Log::out<<"App::createWindow::Creating window"<<Log::newline;
		window->create(appSetup.icon, appSetup.title);

		appSetup.window.bottomright.x = appSetup.window.topleft.x + window->getWidth();
		appSetup.window.bottomright.y = appSetup.window.topleft.y + window->getHeight();


		Log::out<<"App::createWindow::Created window"<<Log::newline;
		{
			class AppKeyListener : public KeyListener {
				App* app;
			public:
				AppKeyListener(App* app)			{			this->app = app;							}
				bool onKeyDown( blib::Key key )		{			app->keyState.setPressed(key, true);	
					if(key == blib::Key::P && app->keyState.isPressed(blib::Key::CONTROL))
						app->showProfiler = !app->showProfiler;
					return false;
				}
				bool onKeyUp(blib::Key key)			{			app->keyState.setPressed(key, false);	return false; }
			};
			addKeyListener(new AppKeyListener(this));
		}

		{
			class AppMouseListener : public MouseListener {
				App* app;
			public:
				AppMouseListener(App* app)										{	this->app = app;								}
				bool onMouseDown(int x, int y, Button button, int clickCount)	{ app->mouseState.clickcount = clickCount; app->mouseState.position.x = x; app->mouseState.position.y = y; app->mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = true; return false; };
				bool onMouseUp(int x, int y, Button button, int clickCount)		{ app->mouseState.clickcount = clickCount; app->mouseState.position.x = x; app->mouseState.position.y = y; app->mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = false; return false; };
				bool onMouseMove(int x, int y, Buttons button)					{ app->mouseState.position.x = x; app->mouseState.position.y = y; return false; };
				bool onScroll(int delta) { app->mouseState.scrollPosition += delta; return false; };
			};
			addMouseListener(new AppMouseListener(this));
		}

		class Resizer : public blib::gl::GlResizeRegister
		{
			App* app;
		public:
			Resizer(App* app) { this->app = app; }
			void resizeGl(int width, int height)
			{
				app->renderer->setViewPort(0, 0, width, height);
			}
		};
		new Resizer(this);

		mouseState.leftButton = false;
		mouseState.rightButton = false;
		mouseState.middleButton = false;
		mouseState.position = glm::ivec2(0, 0);


#ifdef WIN32
		wglSwapIntervalEXT(appSetup.vsync ? 1 : 0);
#else

#endif


		renderer = resourceManager->getResource<Renderer>();
		renderer->app = this;
		//renderState = renderer->renderState;
		spriteBatch = new SpriteBatch(renderer, resourceManager);
		lineBatch = new LineBatch(renderer, resourceManager);

		Log::out<<"calling init..."<<Log::newline;
		init();
		Log::out<<"done calling init..."<<Log::newline;

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
	App::UpdateThread::~UpdateThread()
	{
		delete semaphore;
	}
	App::RenderThread::~RenderThread()
	{
		delete semaphore;
	}



	void App::step()
	{
		if(appSetup.threaded)
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
		else
		{
			if (!gear)
			{
				gear = resourceManager->getResource<Texture>("assets/textures/gear.png");
				//static Texture* white = resourceManager->getResource<Texture>("assets/textures/whitepixel.png");
				font = resourceManager->getResource<Font>("tahoma");
			}
			runRunners();

			renderer->swap();

			window->tick();
			if(joystickDriver)
			{
				joystickDriver->update();
				for(int i = 0; i < 32; i++)
					joyStates[i] = joystickDriver->getJoyState(i);
			}

			double elapsedTime = blib::util::Profiler::getTime();
			blib::util::Profiler::startFrame();
			time += elapsedTime;
			update(elapsedTime);
			if(!running)
				return;
			draw();

			if(showProfiler)
			{
				int frame = ((int)(blib::util::Profiler::getAppTime()*50)) % (12*12);

				spriteBatch->begin();
				spriteBatch->draw(gear, glm::translate(glm::mat4(), glm::vec3(window->getWidth()-80, window->getHeight()-80,0)), glm::vec2(0,0), blib::math::Rectangle(1/12.0f * (frame%12),1/12.0f * (frame/12),1/12.0f,1/12.0f));
				spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::translate(glm::mat4(), glm::vec3(1,1,0)), glm::vec4(0,0,0,1));
				spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::mat4());

				spriteBatch->end();
			}

			renderer->flush();
			window->swapBuffers();

		
        }
	}



	int App::RenderThread::run()
	{
		app->window->makeCurrent();
		
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
		app->window->unmakeCurrent();
		app->semaphore->signal();
		semaphore->wait();
		return 0;
	}

	int App::UpdateThread::run()
	{
		app->createWindow();
		gear = app->resourceManager->getResource<Texture>("assets/textures/gear.png");
		white = app->resourceManager->getResource<Texture>("assets/textures/whitepixel.png");
		font = app->resourceManager->getResource<Font>("tahoma");
		app->window->unmakeCurrent();
		semaphore->signal();
		blib::util::Profiler::startFrame();
		while(app->running)
		{
			semaphore->wait();
			if(!app->running) {
				Log::out<<"I stopped running, Why?"<<Log::newline;
				
				break;
			}
			double frameStart = util::Profiler::getAppTime();
			app->window->tick();
			if(app->joystickDriver)
			{
				app->joystickDriver->update();
				for(int i = 0; i < 32; i++)
					app->joyStates[i] = app->joystickDriver->getJoyState(i);
			}

			app->renderer->setViewPort(0,0,app->window->getWidth(), app->window->getHeight());
			double elapsedTime = blib::util::Profiler::getTime();
			blib::util::Profiler::startFrame();
			app->time += elapsedTime;
			app->runRunners();
			app->update(elapsedTime);
			if(!app->running) {
					
				Log::out<<"I stopped running2, Why?"<<Log::newline;
				app->semaphore->signal();
				break;
			}

			app->draw();

			if(app->showProfiler)
			{
				int frame = ((int)(blib::util::Profiler::getAppTime()*50)) % (12*12);

				app->spriteBatch->begin();
				app->spriteBatch->draw(white, glm::scale(glm::translate(glm::mat4(), glm::vec3(20,20,0)), glm::vec3(200.0f,100.0f,1.0f)), glm::vec2(0,0), blib::math::Rectangle(0,0,1,1), glm::vec4(0,0,0,0.6f));

				app->spriteBatch->draw(gear, glm::translate(glm::mat4(), glm::vec3(app->window->getWidth()-80, app->window->getHeight()-80,0)), glm::vec2(0,0), blib::math::Rectangle(1/12.0f * (frame%12),1/12.0f * (frame/12),1/12.0f,1/12.0f));
				app->spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::translate(glm::mat4(), glm::vec3(1,1,0)), glm::vec4(0,0,0,1));
				app->spriteBatch->draw(font, "FPS: " + util::toString(util::Profiler::fps), glm::mat4());
				app->spriteBatch->draw(font, "draw time",	glm::scale(glm::translate(glm::mat4(), glm::vec3(20, 20,0)), glm::vec3(0.8f, 0.8f, 1)), glm::vec4(1,0,0,1));
				app->spriteBatch->draw(font, "update time", glm::scale(glm::translate(glm::mat4(), glm::vec3(20, 30,0)), glm::vec3(0.8f, 0.8f, 1)), glm::vec4(0,1,0,1));

				app->spriteBatch->end();
				app->lineBatch->renderState.depthTest = false;
				app->lineBatch->begin();
				app->lineBatch->draw(math::Rectangle(glm::vec2(20,20), 200,100), glm::vec4(1,1,1,1));

				PerformanceInfo minValues(99999999999);
				PerformanceInfo maxValues(-99999999999);
				for(int i = 0; i < 1000; i++)
				{
					for(int ii = 0; ii < 3+ profilerCustomProps; ii++)
					{
						minValues.data[ii] = glm::min(minValues.data[ii], app->frameTimes[i].data[ii]);
						maxValues.data[ii] = glm::max(maxValues.data[ii], app->frameTimes[i].data[ii]);
					}
				}

				float timeFactor = 100 / (float)glm::max(maxValues.updateTime, maxValues.drawTime);
				PerformanceInfo prevAccum(0);
				PerformanceInfo accum(0);
				for(int i = 0; i < 1000 && timeFactor < 1e20; i++)
				{
					for(int ii = 0; ii < 3+ profilerCustomProps; ii++)
						accum.data[ii] += app->frameTimes[i].data[ii];
					if(i%5 == 0 && i > 0)
					{
						for(int ii = 0; ii < 3+ profilerCustomProps; ii++)
							accum.data[ii] /= 5.0;

						if(i != 5)
						{
							app->lineBatch->draw(glm::vec2(19 + i*.2f, 120 - timeFactor*prevAccum.drawTime), glm::vec2(20 + i*.2f, 120 - timeFactor*accum.drawTime), glm::vec4(1,0,0,1));
							app->lineBatch->draw(glm::vec2(19 + i*.2f, 120 - timeFactor*prevAccum.updateTime), glm::vec2(20 + i*.2f, 120 - timeFactor*accum.updateTime), glm::vec4(0,1,0,1));

							static glm::vec4 colors[] = { blib::Color::blue, blib::Color::magenta, blib::Color::yellow, blib::Color::cyan };

							for (int ii = 3; ii < 3 + profilerCustomProps; ii++)
								app->lineBatch->draw(glm::vec2(19 + i*.2f, 120 - timeFactor*prevAccum.data[ii]), glm::vec2(20 + i*.2f, 120 - timeFactor*accum.data[ii]), colors[ii-3]);


						}
						prevAccum = accum;
						memset(&accum, 0, sizeof(PerformanceInfo));
					}
				}

				app->lineBatch->end();
			}
			frameTime = util::Profiler::getAppTime() - frameStart;
			app->semaphore->signal();
		}
		semaphore->wait();
		app->semaphore->signal();
		return 0;
	}



	void App::runRunners()
	{
		if (appSetup.threaded || appSetup.backgroundTasks)
			runnerMutex->lock();

		for(std::list<RunnerContainer*>::iterator it = runners.begin(); it != runners.end(); it++)
		{
			(*it)->run();
			delete *it;
		}		
		runners.clear();

		if (appSetup.threaded || appSetup.backgroundTasks)
			runnerMutex->unLock();
	}

	template void App::runLater<int>(std::function<void(int)> toRun, int param);

}

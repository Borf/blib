#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <list>

#include <blib/KeyListener.h>
#include <blib/JoyState.h>
#include <blib/util/Thread.h>
#include <blib/util/Mutex.h>

namespace blib
{
	enum class Key;
	class Window;
	class MouseListener;
	class SpriteBatch;
	class Renderer;
	class RenderState;
	class App;
	class LineBatch;
	class ResourceManager;

	namespace util
	{
		class Mutex;
		class Semaphore;
	}
	namespace drivers
	{
		namespace joystick		{			class Driver;		}
	}



	struct KeyState
	{
		bool pressedKeys[255];
		inline bool isPressed(blib::Key index)
		{
			return pressedKeys[(int)index];
		}
		KeyState()
		{
			ZeroMemory(pressedKeys, sizeof(pressedKeys));
		}
	};
	struct MouseState
	{
		union
		{
			bool buttons[3];
			struct 
			{
				bool leftButton;
				bool middleButton;
				bool rightButton;
			};
		};
		int x;
		int y;
		int clickcount;

		MouseState()
		{
			leftButton = false;
			middleButton = false;
			rightButton = false;
			x = -1;
			y = -1;
			clickcount = 0;
		}
	};

	struct AppSetup
	{
		int width, height;
		bool border;
		bool vsync;
		int icon;
		std::string title;
		bool threaded;
		bool backgroundTasks;
		enum RendererPreference
		{
			NullRenderer,
			GlRenderer,
			DirectGlRenderer,
			DxRenderer,
		} renderer;

		enum JoystickPreference
		{
			NullJoystick,
			WinMM,
			DirectInput,
			XInput,
		} joystickDriver;


		AppSetup()
		{
			width = 1280;
			height = 720;
			border = false;
			vsync = false;
			icon = 0;
			renderer = NullRenderer;
			joystickDriver = NullJoystick;
			title = "Blib App";
			threaded = true;
			backgroundTasks = false;
		}
	};

	class App
	{
	protected:
		
		class RenderThread : public blib::util::Thread
		{
			App* app;
		public:
			RenderThread(App* app);
			int run();
			util::Semaphore* semaphore;
			double frameTime;
		};

		class UpdateThread : public blib::util::Thread
		{
		public:
			App* app;
		public:
			UpdateThread(App* app);
			int run();

			util::Semaphore* semaphore;
			double frameTime;
		};


		double time;

		AppSetup appSetup;


		KeyState keyState;
		MouseState mouseState;
	public:

	protected:
		JoyState joyStates[32];



	public:
		App();
		virtual ~App();
		void start(bool looping = true);
		void run();
		void step();
		void createWindow();


		virtual void init() = 0;
		virtual void update(double elapsedTime) = 0;
		virtual void draw() = 0;
		bool running;

		Window* window;

		template<class T>
		void runLater(std::function<void(T)> toRun, T param)
		{
			if (appSetup.threaded)
				runnerMutex->lock();
			runners.push_back(new RunnerContainerImpl<T>(toRun, param));
			if (appSetup.threaded)
				runnerMutex->unLock();
		}

	protected:
		void addKeyListener(KeyListener* keyListener);
		void addMouseListener(MouseListener* mouseListener);
		


		template<class T>
		friend class BackgroundTask;
	private:
		util::Mutex* runnerMutex;

		class RunnerContainer
		{
		public:
			virtual void run() = 0;
			virtual ~RunnerContainer() {};
		};
		template<class T>
		class RunnerContainerImpl : public RunnerContainer
		{
		public:
			RunnerContainerImpl(std::function<void(T)> function, T param) { this->function = function; this->param = param; }
			std::function<void(T)> function;
			T param;
			void run() { function(param); }
		};
		std::list<RunnerContainer*> runners;
		void runRunners();
	public:

		ResourceManager* resourceManager;

		SpriteBatch* spriteBatch;
		LineBatch* lineBatch;
		Renderer* renderer;

		util::Semaphore* semaphore;
		RenderThread* renderThread;
		UpdateThread* updateThread;
		drivers::joystick::Driver* joystickDriver;

		struct PerformanceInfo
		{
			union 
			{
				struct {
					double updateTime;
					double drawTime;
					double fps;//everything should be double!
				};
				double data[3];
			};
		}frameTimes[1000];
		int frameTimeIndex;

		bool showProfiler;
	};


}

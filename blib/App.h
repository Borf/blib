#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <list>

#include <blib/KeyListener.h>
#include <blib/JoyState.h>
#include <blib/util/Thread.h>
#include <blib/util/Mutex.h>
#include <blib/math/Rectangle.h>

#include <string.h> //memset

#ifdef WIN32
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32s.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"OpenAL32.lib")
#endif

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
	namespace platform
	{
		namespace win32
		{
			class Window;
		}
	}


	// Contains the state of all individual keys on a moment in time
	struct KeyState
	{
	private:
		bool pressedKeys[255];
	public:
		// returns whether or not a key is pressed
		inline bool isPressed(blib::Key index) const		{	return pressedKeys[(int)index];	}
		inline void setPressed(blib::Key index, bool value) { pressedKeys[(int)index] = value; }
		KeyState()
		{
			memset(pressedKeys, 0, sizeof(pressedKeys));
		}
		friend class AppKeyListener;
	};

	// Contains the state of the mouse on a moment in time
	struct MouseState
	{
		static const unsigned char MaxTouches = 10;
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
		
		glm::ivec2 position;
		int clickcount;
		int scrollPosition;

		MouseState()
		{
			leftButton = false;
			middleButton = false;
			rightButton = false;
			position.x = -1;
			position.y = -1;
			clickcount = 0;
			scrollPosition = 0;
		}
	};
	
	struct Touch
	{
		unsigned long id;
		glm::ivec2 position;
		Touch() { id = 0; position.x = 0; position.y = 0; }
		~Touch() {};
	};

	// the settings that are needed to set up the app. Changing these during runtime won't do a thing
	struct AppSetup
	{
		//size and position of the window
		blib::math::Rectangle window;
		//should the window have a border
		bool border;
		//vsync enabled/disabled
		bool vsync;
		//windows: the application icon
		int icon;
		//the window title
		std::string title;
		//enable/disable threading in the rendering
		bool threaded;
		//enable/disable threaded background tasks
		bool backgroundTasks;
		//select the renderer used

		int offX = 0;
		int offY = 0;

		enum RendererPreference
		{
			NullRenderer,
			GlRenderer,
			DirectGlRenderer,
			DxRenderer,
		} renderer;
		//select joystick driver
		enum JoystickPreference
		{
			NullJoystick,
			WinMM,
			DirectInput,
			XInput,
		} joystickDriver;


		AppSetup() : window(0,0,1280,720)
		{
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

	//application class
	class App
	{
	protected:
		//thread to render in background
		class RenderThread : public blib::util::Thread
		{
			App* app;
		public:
			RenderThread(App* app);
			~RenderThread();
			int run();
			util::Semaphore* semaphore;
			double frameTime;
		};

		//thread to update in background
		class UpdateThread : public blib::util::Thread
		{
		public:
			App* app;
		public:
			UpdateThread(App* app);
			~UpdateThread();
			int run();

			util::Semaphore* semaphore;
			double frameTime;
		};

		//basic properties to store the states of things
		double time;
		AppSetup appSetup;
		KeyState keyState;
		MouseState mouseState;
		JoyState joyStates[32];
		ResourceManager* resourceManager;
		SpriteBatch* spriteBatch;
		LineBatch* lineBatch;
		Renderer* renderer;


		//add listeners
		void addKeyListener(KeyListener* keyListener);
		void addMouseListener(MouseListener* mouseListener);

		friend class blib::platform::win32::Window;
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

		//set to false to stop the app
		bool running;
		//the window interface to access the created window
		Window* window;
        Touch touches[10];

		//method to run a method later, but in the updatethread
		template<class T>
		void runLater(std::function<void(T)> toRun, T param)
		{
			if (appSetup.threaded)
				runnerMutex->lock();
			runners.push_back(new RunnerContainerImpl<T>(toRun, param));
			if (appSetup.threaded)
				runnerMutex->unLock();
		}

		template<class T>
		friend class BackgroundTask;
	private:
		//mutex used for void App::runRunners()
		util::Mutex* runnerMutex;
		//base class for runners
		class RunnerContainer
		{
		public:
			virtual void run() = 0;
			virtual ~RunnerContainer() {};
		};
		//the actual runner implementation. This one also has a parameter for the function stored
		template<class T>
		class RunnerContainerImpl : public RunnerContainer
		{
		public:
			RunnerContainerImpl(std::function<void(T)> function, T param) { this->function = function; this->param = param; }
			std::function<void(T)> function;
			T param;
			void run() { function(param); }
		};
		//list of methods that'll be called in the update thread
		std::list<RunnerContainer*> runners;
		void runRunners();

		//performance logging
		struct PerformanceInfo
		{
			union
			{
				double data[10];
				struct {
					double updateTime;
					double drawTime;
					double fps;//everything should be double!
				};
			};
			PerformanceInfo(double d)
			{
				for (int i = 0; i < 10; i++)
					data[i] = d;
			}
			PerformanceInfo()
			{
				for (int i = 0; i < 10; i++)
					data[i] = 0;
			}

		}frameTimes[1000];
		int frameTimeIndex;

		//for multithreaded rendering
		util::Semaphore* semaphore;
		RenderThread* renderThread;
		UpdateThread* updateThread;
		drivers::joystick::Driver* joystickDriver;

	public:

		void setProfileInfo(double d, int index)
		{
			frameTimes[frameTimeIndex].data[3 + index] = d;
		}

		bool showProfiler;
		static int profilerCustomProps;
	};


}

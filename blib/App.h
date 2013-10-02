#pragma once

#include <glm/glm.hpp>

#include <blib/KeyListener.h>
#include <blib/util/Thread.h>

namespace blib
{
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
		class Semaphore;
	}




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


		class JoystickThread : public blib::util::Thread
		{
			App* app;
		public:
			JoystickThread(App* app);
			int run();
		};



		Window* window;
		double time;

		struct AppSetup
		{
			int width, height;
			bool border;
			bool vsync;
			int icon;
			std::string title;
			enum RendererPreference
			{
				NullRenderer,
				GlRenderer,
				DxRenderer,
			} renderer;

			AppSetup()
			{
				width = 1280;
				height = 720;
				border = false;
				vsync = false;
				icon = 0;
				renderer = NullRenderer;
				title = "Blib App";
			}
		} appSetup;


		struct KeyState
		{
			bool pressedKeys[255];
			inline bool isPressed(int index)
			{
				return pressedKeys[index];
			}
		} keyState;
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
		} mouseState;
	public:
		struct JoyState
		{
		public:
			JoyState()
			{
				leftTrigger = 0;
				rightTrigger = 0;
				button = 0;
				connected = false;
			}

			glm::vec2 leftStick;
			glm::vec2 rightStick;

			float leftTrigger;
			float rightTrigger;

			bool connected;
			union
			{
				int button;
				struct 
				{
					unsigned a : 1;
					unsigned b : 1;
					unsigned x : 1;
					unsigned y : 1;

					unsigned l : 1;
					unsigned r : 1;
					unsigned select : 1;
					unsigned start : 1;
					unsigned leftStickButton : 1;
					unsigned rightStickButton : 1;
				};
			};
		};
	protected:
		JoyState joyStates[32];

		bool running;


	public:
		App();
		void start(bool looping = true);
		void run();
		void step();
		void createWindow();


		virtual void init() = 0;
		virtual void update(double elapsedTime) = 0;
		virtual void draw() = 0;


	protected:
		void addKeyListener(KeyListener* keyListener);
		void addMouseListener(MouseListener* mouseListener);
		
		ResourceManager* resourceManager;

		SpriteBatch* spriteBatch;
		LineBatch* lineBatch;
		Renderer* renderer;
		RenderState* renderState;


		util::Semaphore* semaphore;
		RenderThread* renderThread;
		UpdateThread* updateThread;
		JoystickThread* joystickThread;

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
	};


}
#pragma once

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
		};

		class UpdateThread : public blib::util::Thread
		{
		public:
			App* app;
		public:
			UpdateThread(App* app);
			int run();

			util::Semaphore* semaphore;
		};



		Window* window;
		double time;

		struct AppSetup
		{
			int width, height;
			bool border;
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

		bool running;


	public:
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

		SpriteBatch* spriteBatch;
		Renderer* renderer;
		RenderState* renderState;


		util::Semaphore* semaphore;
		RenderThread* renderThread;
		UpdateThread* updateThread;
	};


}
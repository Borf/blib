#pragma once

#include <blib/drivers/joystick/Driver.h>
#include <blib/util/Thread.h>

namespace blib
{
	namespace drivers
	{
		namespace joystick
		{
#ifdef WIN32
			class WinMM : public Driver
			{
				class JoystickThread : public util::Thread
				{
				public:
					JoyState joyStates[32];
					bool running;

					JoystickThread();
					int run();
				};



				JoystickThread* thread;
			public:
				WinMM();
				virtual ~WinMM();

				virtual void update();
				virtual JoyState getJoyState(int id);
			};
#else
			class WinMM : public Driver
			{
			public:
				void update() {};
				JoyState getJoyState(int id) { return JoyState(); };

			};
#endif
		}
	}
}

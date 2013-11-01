#pragma once

#include <blib/drivers/joystick/Driver.h>

namespace blib
{
	namespace drivers
	{
		namespace joystick
		{
#ifdef WIN32
			class WinMM : public Driver
			{
				JoyState joyStates[8];

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

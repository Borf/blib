#pragma once

#include <blib/JoyState.h>

namespace blib
{
	namespace drivers
	{
		namespace joystick
		{
			class Driver
			{
			public:
				virtual ~Driver() {};
				virtual void update() = 0;
				virtual JoyState getJoyState(int id) = 0;
			};
		}
	}
}


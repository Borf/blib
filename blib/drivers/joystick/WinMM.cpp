#include "WinMM.h"


namespace blib
{
	namespace drivers
	{
		namespace joystick
		{
			WinMM::WinMM()
			{

			}

			WinMM::~WinMM()
			{

			}


			void WinMM::update()
			{


			}

			JoyState WinMM::getJoyState(int id)
			{
				return joyStates[id];
			}


		}
	}
}

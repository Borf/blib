#include "WinMM.h"
#include <mmsystem.h>
#include <glm/glm.hpp>

namespace blib
{
	namespace drivers
	{
		namespace joystick
		{

			WinMM::JoystickThread::JoystickThread() : util::Thread("JoystickThread")
			{
			}

			int WinMM::JoystickThread::run()
			{
#ifdef WIN32
				running = true;
				while(running)
				{
					int joystickCount = joyGetNumDevs();
					for(int i = 0; i < glm::min(32,joystickCount); i++)
					{
						JOYINFOEX info;
						MMRESULT res = joyGetPosEx(i, &info);
						//				if(res == JOYERR_UNPLUGGED)
						//					Log::out<<"Unplugged joystick "<<i<<Log::newline;

						JoyState newState;

						if(res != JOYERR_NOERROR)
						{
							joyStates[i].connected = false;
							continue;
						}
						newState.connected = true;
						newState.leftStick.x = 2*(info.dwXpos/65535.0f)-1;
						newState.leftStick.y = 2*(info.dwYpos/65535.0f)-1;

						if(glm::length(newState.leftStick) > 1)
							newState.leftStick = glm::normalize(newState.leftStick);


						float len = glm::length(newState.leftStick);
						float angle = atan2(newState.leftStick.y, newState.leftStick.x);

						if(len >= 0.25f)
							newState.leftStick = glm::vec2((len - 0.25f) * (1.0f / (1-0.25f)) * cos(angle), (len - 0.25f) * (1.0f / (1-0.25f)) * sin(angle));
						else
							newState.leftStick = glm::vec2(0,0);

						newState.leftTrigger = glm::max(0.0f, 2*info.dwZpos/65535.0f-1);
						newState.rightTrigger = glm::max(0.0f, 1-2*info.dwZpos/65535.0f);

						newState.rightStick.x = 2*(info.dwUpos/65535.0f)-1;
						newState.rightStick.y = 2*(info.dwRpos/65535.0f)-1;
						if(glm::length(newState.rightStick) > 1)
							newState.rightStick = glm::normalize(newState.leftStick);

						newState.button = info.dwButtons;
						joyStates[i] = newState;
					}
					Sleep(0);
				}
#endif

				return 0;
			}




			
			
			
			WinMM::WinMM()
			{
				thread = new JoystickThread();
				thread->start();
			}

			WinMM::~WinMM()
			{
				thread->running = false;
				thread->waitForTermination();
			}


			void WinMM::update()
			{
			}

			JoyState WinMM::getJoyState(int id)
			{
				//TODO: this is kind of thread unsafe...
				//return thread->joyStates[id]; // this is kinda thread unsafe
				return JoyState();
			}


}
	}
}

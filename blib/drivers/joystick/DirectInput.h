#pragma once

#include <blib/drivers/joystick/Driver.h>
#include <blib/util/Thread.h>

#ifndef STRICT
#define STRICT
#endif
#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
#include <assert.h>

#include <blib/Window.h>
#include <vector>


namespace blib
{
	namespace drivers
	{
		namespace joystick
		{

			class DirectInput : public Driver
			{
				JoyState joystates[64];

			public:
				struct DI_ENUM_CONTEXT
				{
					DIJOYCONFIG* pPreferredJoyCfg;
					bool bPreferredJoyCfgValid;
				};


				DI_ENUM_CONTEXT			enumContext;
				LPDIRECTINPUT8          g_pDI;

				size_t enumJoyId;
				std::vector<LPDIRECTINPUTDEVICE8>    joysticks;


				DirectInput(blib::Window* window);
				virtual ~DirectInput();

				virtual void update();
				virtual JoyState getJoyState(int id);
			};
		}
	}
}

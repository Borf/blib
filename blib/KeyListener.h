#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

namespace blib
{
	enum Key
	{
#ifdef WIN32
		KEY_ESC = VK_ESCAPE,
#else
		KEY_ESC
#endif

	};

	class KeyListener
	{
	public:
		virtual void onKeyDown(Key key) {};
		virtual void onKeyUp(Key key) {};
		virtual void onChar(char character) {};
	};
}
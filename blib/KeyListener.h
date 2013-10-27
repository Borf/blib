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
		KEY_LEFT = VK_LEFT,
		KEY_RIGHT = VK_RIGHT,
		KEY_UP = VK_UP,
		KEY_DOWN = VK_DOWN,
		KEY_SPACE = VK_SPACE,
		KEY_ENTER = VK_RETURN,
		KEY_HOME = VK_HOME,
		KEY_END = VK_END,
#else
		KEY_ESC,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_SPACE,
		KEY_ENTER,
		KEY_HOME,
		KEY_END,
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

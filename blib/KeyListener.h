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
		KEY_SHIFT = VK_SHIFT,
		KEY_CONTROL = VK_CONTROL,
		KEY_ALT,
		KEY_PLUS = VK_ADD,
		KEY_MINUS = VK_SUBTRACT,
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
		KEY_SHIFT,
		KEY_CONTROL,
		KEY_ALT,
		KEY_PLUS,
		KEY_MINUS,
#endif
	};

	class KeyListener
	{
	public:
		virtual bool onKeyDown(Key key) { return false;  };
		virtual bool onKeyUp(Key key) { return false; };
		virtual bool onChar(char character) { return false; };
	};
}

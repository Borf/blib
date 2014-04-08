#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

namespace blib
{
	enum Key
	{
		KEY_NONE = -1,
#ifdef WIN32
		KEY_ESC =		VK_ESCAPE,
		KEY_LEFT =		VK_LEFT,
		KEY_RIGHT =		VK_RIGHT,
		KEY_UP =		VK_UP,
		KEY_DOWN =		VK_DOWN,
		KEY_SPACE =		VK_SPACE,
		KEY_ENTER =		VK_RETURN,
		KEY_HOME =		VK_HOME,
		KEY_END =		VK_END,
		KEY_SHIFT =		VK_SHIFT,
		KEY_CONTROL =	VK_CONTROL,
		KEY_ALT,
		KEY_PLUS =	VK_ADD,
		KEY_MINUS = VK_SUBTRACT,

		KEY_F1 =	VK_F1,
		KEY_F2 =	VK_F2,
		KEY_F3 =	VK_F3,
		KEY_F4 =	VK_F4,
		KEY_F5 =	VK_F5,
		KEY_F6 =	VK_F6,
		KEY_F7 =	VK_F7,
		KEY_F8 =	VK_F8,
		KEY_F9 =	VK_F9,
		KEY_F10 =	VK_F10,
		KEY_F11 =	VK_F11,
		KEY_F12 =	VK_F12,


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

		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,

#endif
	};

	class KeyListener
	{
	public:
		virtual bool onKeyDown(Key key) { return false;  };
		virtual bool onKeyUp(Key key) { return false; };
		virtual bool onChar(char character) { return false; };


		static Key fromString(std::string);

	};


}

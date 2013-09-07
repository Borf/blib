#pragma once

#include <Windows.h>

namespace blib
{
	enum Key
	{
		KEY_ESC = VK_ESCAPE,

	};

	class KeyListener
	{
	public:
		virtual void onKeyDown(Key key) {};
		virtual void onKeyUp(Key key) {};
		virtual void onChar(char character) {};
	};
}
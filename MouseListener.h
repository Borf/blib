#pragma once

#include <Windows.h>

namespace blib
{
	class MouseListener
	{
	public:
		enum Button
		{
			Left,
			Middle,
			Right,
		};
		struct Buttons
		{
			int button;
			Buttons(int i) : button(i) {};
			inline bool isButtonDown(Button button) { return (button & button) != 0; };
		};

		virtual void onMouseDown(int x, int y, Button button) {};
		virtual void onMouseUp(int x, int y, Button button) {};
		virtual void onMouseMove(int x, int y, Buttons button) {};
	};
}
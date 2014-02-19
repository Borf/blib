#pragma once

namespace blib
{
	class MouseListener
	{
	public:
		enum Button
		{
			Left = 1,
			Middle = 2,
			Right = 4,
		};
		struct Buttons
		{
			int button;
			Buttons(int i) : button(i) {};
			inline bool isButtonDown(Button button) { return (this->button & button) != 0; };
		};

		virtual void onMouseDown(int x, int y, Button button, int clickCount) {};
		virtual void onMouseUp(int x, int y, Button button, int clickCount) {};
		virtual void onMouseMove(int x, int y, Buttons button) {};
		virtual void onScroll(int delta) {};
	};
}
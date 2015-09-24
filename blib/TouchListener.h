#pragma once

namespace blib
{
	class touchListener
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

		virtual bool onMouseDown(int x, int y, Button button, int clickCount) { return false; };
		virtual bool onMouseUp(int x, int y, Button button, int clickCount) { return false; };
		virtual bool onMouseMove(int x, int y, Buttons button) { return false; };
		virtual bool onScroll(int delta) { return false;  };
	};
}
#pragma once
#include <functional>
#include <list>

namespace blib
{
	enum class Key;

	namespace wm
	{
		class KeyboardFocusable
		{
		public:
			typedef std::function<bool(blib::Key key)> keyHandler;
			typedef std::function<bool(char key)> charHandler;
		protected:
			std::list<keyHandler>		keyDownHandlers;
			std::list<keyHandler>		keyUpHandlers;
			std::list<charHandler>		charHandlers;
		public:
			bool canHaveKeyboardFocus = true;

			void addKeyDownHandler(keyHandler handler);
			void addKeyUpHandler(keyHandler handler);
			void addCharHandler(charHandler handler);

			bool onKeyDown(blib::Key key);
			bool onKeyUp(blib::Key key);
			bool onChar(char key);

		};
	}
}


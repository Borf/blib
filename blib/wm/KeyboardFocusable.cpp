#include "KeyboardFocusable.h"
#include <blib/linq.h>

namespace blib
{
	namespace wm
	{

		void KeyboardFocusable::addKeyDownHandler(keyHandler handler)
		{
			keyDownHandlers.push_back(handler);
		}

		void KeyboardFocusable::addKeyUpHandler(keyHandler handler)
		{
			keyUpHandlers.push_back(handler);
		}

		void KeyboardFocusable::addCharHandler(charHandler handler)
		{
			charHandlers.push_back(handler);
		}

		bool KeyboardFocusable::onKeyDown(blib::Key key)
		{
			bool ret = false;
			for (std::list<keyHandler>::iterator it = keyDownHandlers.begin(); it != keyDownHandlers.end(); it++)
			{
				ret |= (*it)(key);
			}
			return ret;
		}

		bool KeyboardFocusable::onKeyUp(blib::Key key)
		{
			bool ret = false;
			for (std::list<keyHandler>::iterator it = keyUpHandlers.begin(); it != keyUpHandlers.end(); it++)
			{
				ret |= (*it)(key);
			}
			return ret;
		}

		bool KeyboardFocusable::onChar(char key)
		{
			bool ret = false;
			for (std::list<charHandler>::iterator it = charHandlers.begin(); it != charHandlers.end(); it++)
			{
				ret |= (*it)(key);
			}
			return ret;
		}

		void KeyboardFocusable::clearKeyboardListeners()
		{
			keyDownHandlers.clear();
			keyUpHandlers.clear();
			charHandlers.clear();
		}

	}
}
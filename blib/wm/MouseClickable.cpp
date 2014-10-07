#include "MouseClickable.h"

namespace blib
{
	namespace wm
	{
		void MouseClickable::addClickHandler(mouseHandler handler)
		{
			clickHandlers.push_back(handler);
		}
		void MouseClickable::addMouseUpHandler(mouseHandler handler)
		{
			mouseUpHandlers.push_back(handler);
		}
		void MouseClickable::addMouseDownHandler(mouseHandler handler)
		{
			mouseDownHandlers.push_back(handler);
		}
		void MouseClickable::addScrollHandler(scrollHandler handler)
		{
			scrollHandlers.push_back(handler);
		}


		bool MouseClickable::onMouseClick(int x, int y, int clickCount)
		{
			bool ret = false;
			for (std::list<mouseHandler>::iterator it = clickHandlers.begin(); it != clickHandlers.end(); it++)
			{
				ret |= (*it)(x, y, clickCount);
			}
			return ret;
		}
		bool MouseClickable::onMouseDown(int x, int y, int clickCount)
		{
			bool ret = false;
			for (std::list<mouseHandler>::iterator it = mouseDownHandlers.begin(); it != mouseDownHandlers.end(); it++)
			{
				ret |= (*it)(x, y, clickCount);
			}
			return ret;
		}
		bool MouseClickable::onMouseUp(int x, int y, int clickCount)
		{
			bool ret = false;
			for (std::list<mouseHandler>::iterator it = mouseUpHandlers.begin(); it != mouseUpHandlers.end(); it++)
			{
				ret |= (*it)(x, y, clickCount);
			}
			return ret;
		}
		bool MouseClickable::onScroll(int x, int y, int delta)
		{
			bool ret = false;
			for (std::list<scrollHandler>::iterator it = scrollHandlers.begin(); it != scrollHandlers.end(); it++)
			{
				ret |= (*it)(x, y, delta);
			}
			return ret;
		}
	}
}

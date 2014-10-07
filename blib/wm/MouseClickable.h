#pragma once
#include <functional>
#include <list>

namespace blib
{
	namespace wm
	{
		class MouseClickable
		{
		public:
			typedef std::function<bool(int x, int y, int clickCount)> mouseHandler;
			typedef std::function<bool(int x, int y, int delta)> scrollHandler;
		protected:
			std::list<mouseHandler>		clickHandlers;
			std::list<mouseHandler>		mouseDownHandlers;
			std::list<mouseHandler>		mouseUpHandlers;
			std::list<scrollHandler>	scrollHandlers;
		public:
			void addClickHandler(mouseHandler handler);
			void addMouseDownHandler(mouseHandler handler);
			void addMouseUpHandler(mouseHandler handler);
			void addScrollHandler(scrollHandler handler);


			bool onMouseClick(int x, int y, int clickCount);
			bool onMouseDown(int x, int y, int clickCount);
			bool onMouseUp(int x, int y, int clickCount);
			bool onScroll(int x, int y, int delta);
		};



	}
}

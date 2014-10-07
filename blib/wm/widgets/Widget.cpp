#include "Widget.h"
#include "../WM.h"
#include "../Window.h"
namespace blib
{
	namespace wm
	{
		Widget::Widget()
		{
			x = 0;
			y = 0;
			width = 50;
			height = 50;

			selected = false;
			hover = false;
			canHaveKeyboardFocus = false;
		}

		Widget::~Widget()
		{
			if(WM::getInstance()->hoverWidget == this)
				WM::getInstance()->hoverWidget = NULL;
			for(std::list<blib::wm::Window*>::iterator it = WM::getInstance()->windows.begin(); it != WM::getInstance()->windows.end(); it++)
				if((*it)->selectedWidget == this)
					(*it)->selectedWidget = NULL;
		}

		Widget* Widget::getComponent( std::string name )
		{
			return this->name == name ? this : NULL;
		}

		bool Widget::inComponent( int x, int y )
		{
			return (x >= this->x && y >= this->y && x < this->x + this->width && y < this->y + this->height);
		}

		Widget* Widget::getComponent(int x, int y)
		{
			return inComponent(x,y) ? this : NULL;
		}
	}
}
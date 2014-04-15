#include "SubMenuMenuItem.h"

namespace blib
{
	namespace wm
	{
		SubMenuMenuItem::SubMenuMenuItem(std::string name, Menu* menu) : MenuItem(name)
		{
			this->menu = menu;
			opened = false;
		}
	}
}

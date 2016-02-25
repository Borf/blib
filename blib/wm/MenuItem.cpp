#include "MenuItem.h"
#include <blib/KeyListener.h>

namespace blib
{
	namespace wm
	{

		MenuItem::MenuItem(std::string name)
		{
			this->name = name;
			this->title = name;
			this->key = Key::NONE;
			this->enabled = true;
		}


	}
}
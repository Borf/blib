#pragma once

#include "MenuItem.h"

namespace blib
{
	namespace wm
	{

		class Menu;

		class SubMenuMenuItem : public MenuItem
		{
		public:
			Menu* menu;
			SubMenuMenuItem(std::string name, Menu* menu);
		};

	}
}
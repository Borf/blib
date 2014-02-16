#pragma once

#include <vector>

namespace blib
{
	namespace wm
	{
		class MenuItem;

		class Menu
		{
		public:
			std::string name;
			std::string description;

			std::vector<MenuItem*> menuItems;
		};
	}
}
#pragma once

#include <vector>

namespace Json { class Value; }

namespace blib
{
	namespace wm
	{
		class MenuItem;

		class Menu
		{
		public:
			Menu(const Json::Value &data);

			std::vector<MenuItem*> menuItems;
		};
	}
}
#pragma once

#include <vector>
#include <functional>

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
			void setAction(std::string path, std::function<void() > callback);
			void linkToggle(std::string, bool* linkBool);
			std::vector<MenuItem*> menuItems;
		};
	}
}
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <blib/json.hpp>


namespace blib
{

	namespace wm
	{
		class MenuItem;

		class Menu
		{
		public:
			Menu(const json &data);
			void setAction(const std::string &path, std::function<void() > callback);
			void linkToggle(const std::string &path, bool* linkBool);
			void setToggleValue(const std::string &path, bool value);
			MenuItem* getItem(const std::string &path);

			std::vector<MenuItem*> getItems(const std::string &path);


			void foreach(std::function<void(MenuItem*)> callback);
			void setMenu(const std::string &menuLoc, blib::wm::MenuItem* menuItem);
			std::vector<MenuItem*> menuItems;

			bool isEnabled(MenuItem* item);
		};
	}
}
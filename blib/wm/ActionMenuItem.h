#pragma once

#include "MenuItem.h"
#include <functional>

namespace blib
{
	namespace wm
	{
		class ActionMenuItem : public MenuItem
		{
		public:
			std::function<void()>	callback;

			ActionMenuItem(std::string name) : MenuItem(name)
			{
				callback = nullptr;
			}
		};
	}
}
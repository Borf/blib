#pragma once

#include <string>

namespace blib
{
	namespace wm
	{
		class MenuItem
		{
		public:
			std::string name;
			std::string description;

			MenuItem(std::string name);
		};
	}
}
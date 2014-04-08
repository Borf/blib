#pragma once

#include <string>

namespace blib
{
	enum Key;

	namespace wm
	{
		class MenuItem
		{
		public:
			std::string name;
			std::string description;
			Key key;

			MenuItem(std::string name);
			virtual ~MenuItem() {}
		};
	}
}
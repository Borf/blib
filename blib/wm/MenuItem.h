#pragma once

#include <string>
#include <blib/KeyListener.h>

namespace blib
{
	enum class Key;

	namespace wm
	{
		class MenuItem
		{
		public:
			std::string name;
			std::string title;
			std::string description;
			Key key;
			bool enabled;
			MenuItem(std::string name);
			virtual ~MenuItem() {}
		};
	}
}

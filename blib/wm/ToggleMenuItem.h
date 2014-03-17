#pragma once

#include "MenuItem.h"

namespace blib
{
	namespace wm
	{
		class ToggleMenuItem : public MenuItem
		{
			bool value;
			bool* attachedValue;
		public:
			ToggleMenuItem(std::string name, bool initialValue) : MenuItem(name)
			{
				value = initialValue;
				attachedValue = NULL;
			}

			bool getValue()
			{
				if (attachedValue)
					return *attachedValue;
				else
					return value;
			}

			void toggle()
			{
				if (attachedValue)
					*attachedValue = !*attachedValue;
				else
					value = !value;
			}

			void linkToggle(bool* linkBool)
			{
				attachedValue = linkBool;
			}



		};
	}
}
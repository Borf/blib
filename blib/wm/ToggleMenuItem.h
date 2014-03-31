#pragma once

#include "ActionMenuItem.h"

namespace blib
{
	namespace wm
	{
		class ToggleMenuItem : public ActionMenuItem
		{
			bool value;
			bool* attachedValue;
		public:
			ToggleMenuItem(std::string name, bool initialValue) : ActionMenuItem(name)
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
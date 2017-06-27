#pragma once

#include "ActionMenuItem.h"

#include <functional>

namespace blib
{
	namespace wm
	{
		class ToggleMenuItem : public ActionMenuItem
		{
			bool value;
			bool* attachedValue = nullptr;
			std::function<bool()> readCallback = nullptr;
			std::function<void(bool)> writeCallback = nullptr;
		public:
			ToggleMenuItem(std::string name, bool initialValue) : ActionMenuItem(name)
			{
				value = initialValue;
			}

			virtual void setValue(bool v)
			{
				if (attachedValue)
					*attachedValue = v;
				else if (writeCallback)
					writeCallback(v);
				else
					value = v;
			}

			virtual bool getValue()
			{
				if (attachedValue)
					return *attachedValue;
				else if (readCallback)
					return readCallback();
				else
					return value;
			}

			virtual void toggle()
			{
				if (attachedValue)
					*attachedValue = !*attachedValue;
				else if (readCallback && writeCallback)
					writeCallback(!readCallback());
				else
					value = !value;
			}

			virtual void linkToggle(bool* linkBool)
			{
				attachedValue = linkBool;
			}



		};
	}
}
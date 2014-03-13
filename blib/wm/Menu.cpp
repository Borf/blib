#include "Menu.h"

#include "SubMenuMenuItem.h"
#include "MenuItem.h"
#include "ToggleMenuItem.h"
#include "ActionMenuItem.h"


#include <blib/util/Log.h>
using blib::util::Log;

#include <json/value.h>


blib::wm::Menu::Menu(const Json::Value &data)
{
	for (Json::ArrayIndex i = 0; i < data.size(); i++)
	{
		MenuItem* subItem = NULL;

		if (data[i]["type"].asString() == "menu")
		{
			subItem = new SubMenuMenuItem(data[i]["name"].asString(), new Menu(data[i]["subitems"]));
		}
		else if (data[i]["type"].asString() == "item")
		{
			subItem = new ActionMenuItem(data[i]["name"].asString());
		}
		else
			Log::err << "Unknown menu type: " << data[i]["type"].asString() << Log::newline;

		if (subItem)
			menuItems.push_back(subItem);
	}
}


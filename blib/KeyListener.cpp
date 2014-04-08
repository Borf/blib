#include "KeyListener.h"
#include <map>
#include <algorithm>

namespace blib
{
	Key KeyListener::fromString(std::string name)
	{
		static std::map<std::string, Key>  keyMapping;
		if (keyMapping.empty())
		{
			keyMapping["a"] = (Key)'A';
			keyMapping["b"] = (Key)'B';
			keyMapping["c"] = (Key)'C';
			keyMapping["d"] = (Key)'D';
			keyMapping["e"] = (Key)'E';
			keyMapping["f"] = (Key)'F';
			keyMapping["g"] = (Key)'G';
			keyMapping["h"] = (Key)'H';
			keyMapping["i"] = (Key)'I';
			keyMapping["j"] = (Key)'J';
			keyMapping["k"] = (Key)'K';
			keyMapping["l"] = (Key)'L';
			keyMapping["m"] = (Key)'M';
			keyMapping["n"] = (Key)'N';
			keyMapping["o"] = (Key)'O';
			keyMapping["p"] = (Key)'P';
			keyMapping["q"] = (Key)'Q';
			keyMapping["r"] = (Key)'R';
			keyMapping["s"] = (Key)'S';
			keyMapping["t"] = (Key)'T';
			keyMapping["u"] = (Key)'U';
			keyMapping["v"] = (Key)'V';
			keyMapping["w"] = (Key)'W';
			keyMapping["x"] = (Key)'X';
			keyMapping["y"] = (Key)'Y';
			keyMapping["z"] = (Key)'Z';

			keyMapping["f1"] = KEY_F1;
			keyMapping["f2"] = KEY_F2;
			keyMapping["f3"] = KEY_F3;
			keyMapping["f4"] = KEY_F4;
			keyMapping["f5"] = KEY_F5;
			keyMapping["f6"] = KEY_F6;
			keyMapping["f7"] = KEY_F7;
			keyMapping["f8"] = KEY_F8;
			keyMapping["f9"] = KEY_F9;
			keyMapping["f10"] = KEY_F10;
			keyMapping["f11"] = KEY_F11;
			keyMapping["f12"] = KEY_F12;

		}

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		return keyMapping[name];

	}

}
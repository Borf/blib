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

			keyMapping["f1"] = Key::F1;
			keyMapping["f2"] = Key::F2;
			keyMapping["f3"] = Key::F3;
			keyMapping["f4"] = Key::F4;
			keyMapping["f5"] = Key::F5;
			keyMapping["f6"] = Key::F6;
			keyMapping["f7"] = Key::F7;
			keyMapping["f8"] = Key::F8;
			keyMapping["f9"] = Key::F9;
			keyMapping["f10"] = Key::F10;
			keyMapping["f11"] = Key::F11;
			keyMapping["f12"] = Key::F12;

			keyMapping["1"] = (Key)'1';
			keyMapping["2"] = (Key)'2';
			keyMapping["3"] = (Key)'3';
			keyMapping["4"] = (Key)'4';
			keyMapping["5"] = (Key)'5';
			keyMapping["6"] = (Key)'6';
			keyMapping["7"] = (Key)'7';
			keyMapping["8"] = (Key)'8';
			keyMapping["9"] = (Key)'9';
			keyMapping["0"] = (Key)'0';
		}

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		return keyMapping[name];

	}

}
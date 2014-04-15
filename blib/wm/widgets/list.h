#pragma once

#include "Widget.h"

#include <vector>
#include <string>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			class List : public Widget
			{
				std::vector<int> selectedItems;
			public:
				List();
				bool multiselect;


				int scrollPosition;
				int selectedItem();
				bool selectedItem(int item);
				std::vector<std::string> items;



				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix);

				virtual void mousewheel( int direction, int x, int y );
				virtual void mouseclick(int x, int y, int clickcount);

				virtual void keyboardSpecial(int key);
			};
		}
	}
}
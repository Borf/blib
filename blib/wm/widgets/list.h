#pragma once

#include <blib/blib.h>
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
			public:
				List();
				bool multiselect;


				int scrollPosition;
				int selectedItem() const;
				bool selectedItem(int item) const;
				std::vector<int> selectedItems;
				std::vector<std::string> items;



				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
			};
		}
	}
}
#pragma once

#include "Widget.h"

#include <list>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{

			class ContainerWidget : public Widget
			{
			protected:
				bool checkVisibility;
			public:
				std::list<Widget*> children;

				ContainerWidget();

				void add(Widget* toAdd);
				void clear();

				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;
				virtual Widget* getComponent(std::string name);
				virtual Widget* getComponent(int x, int y);

			};
		}
	}

}

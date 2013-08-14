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
				std::list<Widget*> children;
			public:
				void add(Widget* toAdd);
				void clear();

				virtual void draw(gl::SpriteBatch &spriteBatch, glm::mat4 matrix);
				virtual Widget* getComponent(std::string name);
				virtual Widget* getComponent(int x, int y);
				virtual void mousewheel( int direction, int x, int y );
				virtual void mousedown( int x, int y );
				virtual void mouseup( int x, int y );
				virtual void mouseclick( int x, int y, int clickcount );
				virtual void mousedrag(int x, int y);


			};
		}
	}

}

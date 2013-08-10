#include "Panel.h"
#include <glm/gtc/matrix_transform.hpp>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			void Panel::draw( gl::SpriteBatch &spriteBatch, glm::mat4 matrix)
			{
				ContainerWidget::draw(spriteBatch, glm::translate(matrix, glm::vec3(x,y,0)));
			}

			void Panel::arrangeComponents( int oldWidth, int oldHeight )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					Widget* w = *it;
					if(w->positionHelpLeft == Widget::RIGHT)
						w->x += width-oldWidth;
					else if(w->positionHelpRight == Widget::RIGHT)
						w->width += width-oldWidth;


					if(w->positionHelpTop == Widget::BOTTOM)
						w->y += height-oldHeight;
					else if(w->positionHelpBottom== Widget::BOTTOM)
						w->height += height-oldHeight;
		
					//if(w->positionHelpLeft == Widget::RIGHT)
					//	w->x += width-oldWidth;

				}
			}

		}
	}
}
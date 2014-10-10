#include "ContainerWidget.h"


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			ContainerWidget::ContainerWidget()
			{
				checkVisibility = true;

				addMouseDownHandler([this](int x, int y, int clickcount) 
				{ 
					for (std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
						if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
							if ((*it)->inComponent(x - this->x, y - this->y))
								return (*it)->onMouseDown(x - this->x, y - this->y, clickcount);
					return false;
				});

				addMouseUpHandler([this](int x, int y, int clickcount)
				{
					for (std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
						if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
							if ((*it)->inComponent(x - this->x, y - this->y))
								return (*it)->onMouseUp(x - this->x, y - this->y, clickcount);
					return false;
				});

				addClickHandler([this](int x, int y, int clickcount)
				{
					for (std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
						if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
							if ((*it)->inComponent(x - this->x, y - this->y))
							return (*it)->onMouseClick(x - this->x, y - this->y, clickcount);
					return false;
				});

				addScrollHandler([this](int x, int y, int delta) {
					for (std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
						if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
							if ((*it)->inComponent(x - this->x, y - this->y))
								return (*it)->onScroll(x - this->x, y - this->y, delta);
					return false;
				});
			}


			void ContainerWidget::add( Widget* toAdd )
			{
				children.push_back(toAdd);
			}

			void ContainerWidget::clear()
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
					delete *it;
				children.clear();
			}


			void ContainerWidget::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				for(std::list<Widget*>::const_iterator it = children.begin(); it != children.end(); it++)
				{
					if((*it)->x < width && (*it)->y < height && (*it)->x+(*it)->width > 0 && (*it)->y+(*it)->height > 0)
						(*it)->draw(spriteBatch, matrix, renderer);
				}
			}

			Widget* ContainerWidget::getComponent( std::string name )
			{
				if(this->name == name)
					return this;
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					Widget* ret = (*it)->getComponent(name);
					if(ret)
						return ret;
				}
				return NULL;
			}

			Widget* ContainerWidget::getComponent( int x, int y )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if((*it)->x < width && (*it)->y < height && (*it)->x+(*it)->width > 0 && (*it)->y+(*it)->height > 0)
					{
						Widget* widget = (*it)->getComponent(x-this->x, y - this->y);
						if(widget)
							return widget;
					}
				}

				if(inComponent(x, y))
					return this;
				else
					return NULL;
			}

		}
	}
}
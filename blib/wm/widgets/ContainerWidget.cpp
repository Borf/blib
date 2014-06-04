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

			void ContainerWidget::mousewheel( int direction, int x, int y )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if(!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x+(*it)->width > 0 && (*it)->y+(*it)->height > 0))
						if((*it)->inComponent(x-this->x, y - this->y))
						{
							(*it)->mousewheel(direction, x-this->x, y-this->y);
							return;
						}
				}
			}

			void ContainerWidget::mousedown(int x, int y )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
						if((*it)->inComponent(x-this->x, y - this->y))
						{
							(*it)->mousedown(x-this->x, y-this->y);
							return;
						}
				}
			}
			void ContainerWidget::mouseup(int x, int y )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
						if((*it)->inComponent(x-this->x, y - this->y))
						{
							(*it)->mouseup(x-this->x, y-this->y);
							return;
						}
				}
			}
			void ContainerWidget::mousedrag(int x, int y )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
						if((*it)->inComponent(x-this->x, y - this->y))
						{
							(*it)->mousedrag(x-this->x, y-this->y);
							return;
						}
				}
			}

			void ContainerWidget::mouseclick(int x, int y, int clickcount )
			{
				for(std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					if (!checkVisibility || ((*it)->x < width && (*it)->y < height && (*it)->x + (*it)->width > 0 && (*it)->y + (*it)->height > 0))
						if((*it)->inComponent(x-this->x, y - this->y))
						{
							(*it)->mouseclick(x-this->x, y-this->y, clickcount);
							return;
						}
				}
			}
		}
	}
}
#include "WM.h"

#include "Window.h"
#include <blib/SpriteBatch.h>
#include <blib/Texture.h>
#include <blib/Font.h>
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
using blib::util::Log;

#include <cctype>
#include <algorithm>
#include <windows.h>

namespace blib
{
	namespace wm
	{

		WM::WM()
		{
			draggingWindow = NULL;
			leftbuttondown = false;
			hoverWidget = NULL;
			clickWidget = NULL;
			currentCursor = ARROW;
		}


		WM::~WM()
		{
		}

		void WM::addWindow( Window* window )
		{
			windows.push_front(window);
		}


		void WM::removeWindow(Window* window)
		{
			window->enabled = false;
		}

		bool WM::hasModalWindow()
		{
			if(windows.empty())
				return false;

			return windows.front()->modal;
	
		}

		void WM::draw( SpriteBatch &spriteBatch )
		{
			for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				if(!(*it)->enabled)
				{
					delete *it;
					windows.erase(it);
					if(windows.empty())
						break;
					it = windows.begin();
				}
			}

			if(windows.empty())
				return;

			spriteBatch.begin();
			if(hasModalWindow())
			{
/*				shader->setMatrix(glm::mat4());
				shader->useTexture(false);
				shader->setColor(glm::vec4(0,0,0,0.5f));
				glBegin(GL_QUADS);
					glVertexAttrib3f(0, 0, 0, 0);
					glVertexAttrib3f(0, (float)shader->width, 0, 0);
					glVertexAttrib3f(0, (float)shader->width, (float)shader->height, 0);
					glVertexAttrib3f(0, 0, (float)shader->height, 0);
				glEnd();*/
			}

//			shader->setColor(glm::vec4(1,1,1,1));
			for(std::list<Window*>::reverse_iterator it = windows.rbegin(); it != windows.rend(); it++)
			{
				if((*it)->visible)
					(*it)->draw(spriteBatch);
			}
			spriteBatch.end();
		}

		void WM::setSkin( std::string skinFile )
		{
			skin = util::FileSystem::getJson(skinFile);
			skinTexture = Texture::loadCached(skin["texture"].asString());
			font = Font::getFontInstance("tahoma");
		}

		void WM::setFont( Font* font )
		{
			this->font = font;
		}


		#define DRAGRANGE 10

		bool WM::mousemove( int oldX, int oldY, int x, int y )
		{
			bool handled = false;

			if(draggingWindow == NULL)
			{
				Cursor newCursor = ARROW;
				for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if(w->inWindow(x, y) && w->visible)
					{
						dragMode= 0;//left = 1, right = 2, top = 4, bottom = 8
						if(w->resizable)
						{
							if(abs(x - w->x) < DRAGRANGE)
								dragMode |= 1;
							if(abs(x - (w->x + w->width)) < DRAGRANGE)
								dragMode |= 2;
							if(abs(y - w->y) < DRAGRANGE)
								dragMode |= 4;
							if(abs(y - (w->y + w->height)) < DRAGRANGE)
								dragMode |= 8;

							if(dragMode == 1 || dragMode == 2)
								newCursor = RESIZE_H;
							else if(dragMode == 4 || dragMode == 8)
								newCursor = RESIZE_V;
							else if(dragMode == (1 | 4)) // topleft
								newCursor = RESIZE_TL;
							else if(dragMode == (1 | 8)) // bottomleft
								newCursor = RESIZE_BL;
							else if(dragMode == (2 | 4)) // topright
								newCursor = RESIZE_BL;
							else if(dragMode == (2 | 8)) // bottomright
								newCursor = RESIZE_TL;
						}
						handled = true;
						break;
					}
				}

				//if(currentCursor != newCursor)
				{
					currentCursor = newCursor;
					if(currentCursor == ARROW)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
					else if(currentCursor == RESIZE_H)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
					else if(currentCursor == RESIZE_V)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					else if(currentCursor == RESIZE_TL)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
					else if(currentCursor == RESIZE_BL)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));


				}

			}

			if(leftbuttondown && draggingWindow != NULL)
			{
				if(dragMode == 0 && draggingWindow->movable)
				{//move
					draggingWindow->x += oldX-x;
					draggingWindow->y += oldY-y;
				}
				if(draggingWindow->resizable)
				{
					int oldWidth = draggingWindow->width;
					int oldHeight = draggingWindow->height;
					if((dragMode & 1) != 0)//left
					{
						int rightX = draggingWindow->x + draggingWindow->width;
						draggingWindow->x = x;
						draggingWindow->width = rightX - draggingWindow->x;
					}
					if((dragMode & 2) != 0)//right
						draggingWindow->width = x - draggingWindow->x;
					if((dragMode & 4) != 0)//top
					{
						int bottomY = draggingWindow->y + draggingWindow->height;
						draggingWindow->y = y;
						draggingWindow->height = bottomY - draggingWindow->y;
					}
					if((dragMode & 8) != 0)//bottom
						draggingWindow->height = y - draggingWindow->y;
					draggingWindow->arrangeComponents(oldWidth, oldHeight);
				}
				handled = true;
			}

			if(leftbuttondown)
			{
				for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if(w->inWindow(x, y) && w->visible)
					{
						w->mousedrag(x,y);
						handled = true;
						break;
					}
				}
			}

			if(!leftbuttondown)
			{
				if(hoverWidget != NULL)
					hoverWidget->hover = false;
				for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if(w->inWindow(x, y) && w->visible)
					{
						Widget* widget = w->getComponent(x,y);
						if(widget != NULL)
							widget->hover = true;
						hoverWidget = widget;
						handled = true;
						break;
					}
				}
			}

			return handled;
		}

		bool WM::mousedown( int x, int y )
		{
			leftbuttondown = true;

			for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if(w->inWindow(x, y) && w->visible)
				{
					if(w->inComponent(x,y))
					{
						w->mousedown(x, y);
						clickX = x;
						clickY = y;
					}
					else
						draggingWindow = w;
					return true;
				}
			}
			return false;
		}

		bool WM::mouseup( int x, int y, int clickcount )
		{
			leftbuttondown = false;

			if(draggingWindow != NULL)
				draggingWindow = NULL;

			for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if(w->inWindow(x, y) && w->visible)
				{
					w->mouseup(x, y);
					if(abs(clickX - x) < 3 && abs(clickY - y) < 3)
					{
						w->mouseclick(x, y, clickcount);
					}
					return true;
				}
			}
			return false;
		}

		bool WM::mousewheel( int direction, int x, int y )
		{
			for(std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if(w->inWindow(x, y) && w->visible)
				{
					w->mousewheel(direction, x, y);
					return true;
				}
				if(it == windows.begin() && w->modal)
					return true;
			}
			return false;
		}


		bool WM::keyboard(char key)
		{
			if(windows.empty())
				return false;
			windows.front()->keyboard(key);
			return true;
		}


		bool WM::keyboardSpecial(int key)
		{
			if(windows.empty())
				return false;
			windows.front()->keyboardSpecial(key);
			return true;
		}


		glm::vec4 WM::convertHexColor4( std::string hexColor )
		{
			std::transform(hexColor.begin(), hexColor.end(), hexColor.begin(), std::tolower);
			static std::string digits = "0123456789abcdef";
			float r = (digits.find(hexColor[0])<<4 | digits.find(hexColor[1])) / 255.0f;
			float g = (digits.find(hexColor[2])<<4 | digits.find(hexColor[3])) / 255.0f;
			float b = (digits.find(hexColor[4])<<4 | digits.find(hexColor[5])) / 255.0f;
			return glm::vec4(r,g,b,1);
		}


	}
}

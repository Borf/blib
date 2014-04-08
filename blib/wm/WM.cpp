#include "WM.h"

#include "Window.h"
#include <blib/SpriteBatch.h>
#include <blib/Texture.h>
#include <blib/Font.h>
#include <blib/ResourceManager.h>
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
#include <blib/wm/Menu.h>
#include <blib/Math.h>
#include <blib/wm/MenuItem.h>
#include <blib/wm/ActionMenuItem.h>
#include <blib/wm/SubMenuMenuItem.h>
#include <blib/wm/ToggleMenuItem.h>
using blib::util::Log;

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cctype>
#include <algorithm>
#ifdef WIN32
#include <windows.h>
#endif

namespace blib
{
	namespace wm
	{

		WM::WM()
		{
			draggingWindow = NULL;
			hoverWidget = NULL;
			clickWidget = NULL;
			currentCursor = ARROW;

			radialMenuRoot = NULL;
			radialMenu = NULL;
			pulldownMenu = NULL;
			popupMenu = NULL;
		}


		WM::~WM()
		{
		}

		void WM::addWindow( Window* window )
		{
			windows.push_front(window);
			Log::out << "Added new window '"<<window->title<<"', windowcount = " << windows.size() << Log::newline;
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
			for(std::list<fastdelegate::FastDelegate0<> >::iterator it = tasks.begin(); it != tasks.end(); it++)
				(*it)();

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

			if (!windows.empty())
			{
				if (hasModalWindow())
					spriteBatch.drawStretchyRect(skinTexture, glm::mat4(), WM::getInstance()->skin["button"], glm::vec2(screenSize), glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));

				for (std::list<Window*>::reverse_iterator it = windows.rbegin(); it != windows.rend(); it++)
				{
					if ((*it)->visible)
						(*it)->draw(spriteBatch);
				}
			}

			if (radialMenu)
			{

				glm::vec2 diff = radialMenuPosition - glm::vec2(mouseState.x, mouseState.y);
				float angle = atan2(diff.y, diff.x);
				int id = (int)round(((angle+2*M_PI) / (2 * M_PI)) * 8+7) % 8;


				for (int i = 0; i < 8; i++)
					spriteBatch.draw(skinTexture, blib::math::easyMatrix(radialMenuPosition, 22.5f + i * 45), glm::vec2(126, 90), blib::math::Rectangle(glm::vec2(89.0f / skinTexture->originalWidth, 0), 126.0f / skinTexture->originalWidth, 90.0f / skinTexture->originalHeight), i == id ? glm::vec4(0.9f, 0.5f, 0.5f, 1.0f): (i % 2 == 0 ? glm::vec4(0.9f, 0.9f, 0.9f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

				for (size_t i = 0; i < 8; i++)
				{
					
					if (i < radialMenu->menuItems.size())
					{
						glm::mat4 matrix;
						matrix = glm::translate(matrix, glm::vec3(radialMenuPosition, 0));
						matrix = glm::rotate(matrix, i * 45.0f, glm::vec3(0, 0, 1));
						matrix = glm::translate(matrix, glm::vec3(-120, 0, 0));
						matrix = glm::scale(matrix, glm::vec3(0.75f, 0.75f, 1.0f));
						matrix = glm::translate(matrix, glm::vec3(0, -16, 0));


						ToggleMenuItem* toggle = dynamic_cast<ToggleMenuItem*>(radialMenu->menuItems[i]);
						if (toggle)
						{
							spriteBatch.draw(radialmenufont, (toggle->getValue() ? "[x]  " : "[  ]  ") + radialMenu->menuItems[i]->name, matrix, glm::vec4(0, 0, 0, 1));
						}
						else
							spriteBatch.draw(radialmenufont, radialMenu->menuItems[i]->name, matrix, glm::vec4(0, 0, 0, 1));


					}
				}
			}



			prevMouseState = mouseState; // TODO: move this to an update method?
		}

		void WM::setSkin( std::string skinFile, ResourceManager* resourceManager )
		{
			skin = util::FileSystem::getJson(skinFile);
			skinTexture = resourceManager->getResource<Texture>(skin["texture"].asString());
			font = resourceManager->getResource<Font>("tahoma");
			radialmenufont = resourceManager->getResource<Font>(skin["radialfont"].asString());
		}

		void WM::setFont( Font* font )
		{
			this->font = font;
		}


		#define DRAGRANGE 10



		glm::vec4 WM::convertHexColor4( std::string hexColor )
		{
			std::transform(hexColor.begin(), hexColor.end(), hexColor.begin(), ::tolower);
			static std::string digits = "0123456789abcdef";
			float r = (digits.find(hexColor[0])<<4 | digits.find(hexColor[1])) / 255.0f;
			float g = (digits.find(hexColor[2])<<4 | digits.find(hexColor[3])) / 255.0f;
			float b = (digits.find(hexColor[4])<<4 | digits.find(hexColor[5])) / 255.0f;
			return glm::vec4(r,g,b,1);
		}

		void WM::addTask( fastdelegate::FastDelegate0<> task )
		{
			tasks.push_back(task);
		}

		blib::wm::Menu* WM::loadMenu( std::string filename )
		{
			Json::Value menuData = blib::util::FileSystem::getJson(filename);

			Menu* root = new Menu(menuData);

			root->foreach([this](MenuItem* item) {
				if (item->key != KEY_NONE)
				{
					menuKeys[item->key] = item;
				}
			});

			return root;
		}

		void WM::setRadialMenu( blib::wm::Menu* menu )
		{
			this->radialMenuRoot = menu;
		}

		bool WM::onMouseDown(int x, int y, Button button, int clickCount)
{
			mouseState.x = x; 
			mouseState.y = y; 
			mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = true; 


			if (radialMenu)
			{
				glm::vec2 diff = radialMenuPosition - glm::vec2(mouseState.x, mouseState.y);
				float angle = atan2(diff.y, diff.x);
				int id = (int)round(((angle + 2 * M_PI) / (2 * M_PI)) * 8) % 8;
				if (id < (int)radialMenu->menuItems.size())
				{
					SubMenuMenuItem* submenuitem = dynamic_cast<SubMenuMenuItem*>(radialMenu->menuItems[id]);
					if (submenuitem)
					{
						radialMenu = submenuitem->menu;
						radialMenuPosition = glm::vec2(mouseState.x, mouseState.y);
						return true;
					}

					{
						ToggleMenuItem* item = dynamic_cast<ToggleMenuItem*>(radialMenu->menuItems[id]);
						if (item)
							item->toggle();
					}

					{
						ActionMenuItem* item = dynamic_cast<ActionMenuItem*>(radialMenu->menuItems[id]);
						if (item)
						{
							radialMenu = NULL;
							if (item->callback)
								item->callback();
							return true;
						}
					}
				}
				return true;
			}
			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if (w->inWindow(x, y) && w->visible)
				{
					if (w->inComponent(x, y))
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
		bool WM::onMouseUp(int x, int y, Button button, int clickCount)
{
			mouseState.x = x; 
			mouseState.y = y; 
			mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = false; 

			if (draggingWindow != NULL)
				draggingWindow = NULL;

			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if (w->inWindow(x, y) && w->visible)
				{
					w->mouseup(x, y);
					if (abs(clickX - x) < 3 && abs(clickY - y) < 3)
					{
						w->mouseclick(x, y, clickCount);
					}
					return true;
				}
			}
			return false;
		}
		
		bool WM::onMouseMove(int x, int y, Buttons button)
{
			mouseState.x = x; 
			mouseState.y = y; 

			bool handled = false;

			if (draggingWindow == NULL)
			{
				Cursor newCursor = ARROW;
				for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if (w->inWindow(x, y) && w->visible)
					{
						dragMode = 0;//left = 1, right = 2, top = 4, bottom = 8
						if (w->resizable)
						{
							if (abs(x - w->x) < DRAGRANGE)
								dragMode |= 1;
							if (abs(x - (w->x + w->width)) < DRAGRANGE)
								dragMode |= 2;
							if (abs(y - w->y) < DRAGRANGE)
								dragMode |= 4;
							if (abs(y - (w->y + w->height)) < DRAGRANGE)
								dragMode |= 8;

							if (dragMode == 1 || dragMode == 2)
								newCursor = RESIZE_H;
							else if (dragMode == 4 || dragMode == 8)
								newCursor = RESIZE_V;
							else if (dragMode == (1 | 4)) // topleft
								newCursor = RESIZE_TL;
							else if (dragMode == (1 | 8)) // bottomleft
								newCursor = RESIZE_BL;
							else if (dragMode == (2 | 4)) // topright
								newCursor = RESIZE_BL;
							else if (dragMode == (2 | 8)) // bottomright
								newCursor = RESIZE_TL;
						}
						handled = true;
						break;
					}
				}

				//if(currentCursor != newCursor)
				{
					currentCursor = newCursor;
#ifdef WIN32 //todo: make abstract thingy here
					if (currentCursor == ARROW)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
					else if (currentCursor == RESIZE_H)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
					else if (currentCursor == RESIZE_V)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					else if (currentCursor == RESIZE_TL)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
					else if (currentCursor == RESIZE_BL)
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
#endif

				}

			}

			if (mouseState.leftButton && draggingWindow != NULL)
			{
				if (dragMode == 0 && draggingWindow->movable)
				{//move
					draggingWindow->x += x-prevMouseState.x;
					draggingWindow->y += y-prevMouseState.y;
				}
				if (draggingWindow->resizable)
				{
					int oldWidth = draggingWindow->width;
					int oldHeight = draggingWindow->height;
					if ((dragMode & 1) != 0)//left
					{
						int rightX = draggingWindow->x + draggingWindow->width;
						draggingWindow->x = x;
						draggingWindow->width = rightX - draggingWindow->x;
					}
					if ((dragMode & 2) != 0)//right
						draggingWindow->width = x - draggingWindow->x;
					if ((dragMode & 4) != 0)//top
					{
						int bottomY = draggingWindow->y + draggingWindow->height;
						draggingWindow->y = y;
						draggingWindow->height = bottomY - draggingWindow->y;
					}
					if ((dragMode & 8) != 0)//bottom
						draggingWindow->height = y - draggingWindow->y;
					draggingWindow->arrangeComponents(oldWidth, oldHeight);
				}
				handled = true;
			}

			if (mouseState.leftButton)
			{
				for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if (w->inWindow(x, y) && w->visible)
					{
						w->mousedrag(x, y);
						handled = true;
						break;
					}
				}
			}

			if (!mouseState.leftButton)
			{
				if (hoverWidget != NULL)
					hoverWidget->hover = false;
				for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
				{
					Window* w = (*it);
					if (w->inWindow(x, y) && w->visible)
					{
						Widget* widget = w->getComponent(x, y);
						if (widget != NULL)
							widget->hover = true;
						hoverWidget = widget;
						handled = true;
						break;
					}
				}
			}

			return handled;
		}



		bool WM::onScroll(int delta)
{
			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if (w->inWindow(mouseState.x, mouseState.y) && w->visible)
				{
					w->mousewheel(delta, mouseState.x, mouseState.y);
					return true;
				}
				if (it == windows.begin() && w->modal)
					return true;
			}
			return false;
		}

		bool WM::onKeyDown(Key key)
		{
			if (key == KEY_SPACE)
			{
				radialMenu = radialMenu ? NULL : radialMenuRoot;
				radialMenuPosition.x = (float)mouseState.x;
				radialMenuPosition.y = (float)mouseState.y;
			}

			if (menuKeys.find(key) != menuKeys.end())
			{
				{
					ToggleMenuItem* item = dynamic_cast<ToggleMenuItem*>(menuKeys[key]);
					if (item)
						item->toggle();
				}

				{
					ActionMenuItem* item = dynamic_cast<ActionMenuItem*>(menuKeys[key]);
					if (item)
					{
						if (item->callback)
							item->callback();
					}
				}
			}

			/*if (windows.empty())
				return false;
			windows.front()->keyboard(key);*/
			return false;
		}

		bool WM::onKeyUp(Key key)
		{
		/*	if (windows.empty())
				return false;
			windows.front()->keyboard(key);
			return true;*/
			return false;
		}

		bool WM::onChar(char character)
		{
			if (windows.empty())
				return false;
			windows.front()->keyboard(character);
			return true;
		}

		void WM::resizeGl(int width, int height)
		{
			screenSize.x = width;
			screenSize.y = height;
		}

		void WM::center(Window* window)
		{
			window->x = (screenSize.x - window->width) / 2;
			window->y = (screenSize.y - window->height) / 2;
		}

		bool WM::inWindow(int x, int y)
		{
			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				if ((*it)->inWindow(x, y) && (*it)->visible)
					return true;
			}
			return false;
		}


	}
}

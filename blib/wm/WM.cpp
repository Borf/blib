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
			popupMenu = NULL;
			radialMenu = NULL;
			menuBar = NULL;
			menuBarOpen = false;

			keyPressed = false;
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

		void WM::draw(SpriteBatch &spriteBatch, Renderer* renderer)
		{
			for(std::list<std::function<void()> >::iterator it = tasks.begin(); it != tasks.end(); it++)
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
						(*it)->draw(spriteBatch, renderer);
				}
			}

			if (radialMenu)
			{

				glm::vec2 diff = radialMenuPosition - glm::vec2(mouseState.position);
				float angle = atan2(diff.y, diff.x);
                int id = (int)glm::round(((angle+2*blib::math::pif) / (2 * blib::math::pif)) * 8+7) % 8;


				for (int i = 0; i < 8; i++)
					spriteBatch.draw(skinTexture, blib::math::easyMatrix(radialMenuPosition, 22.5f + i * 45), glm::vec2(126, 90), blib::math::Rectangle(glm::vec2(89.0f / skinTexture->originalWidth, 0), 126.0f / skinTexture->originalWidth, 90.0f / skinTexture->originalHeight), i == id ? glm::vec4(0.9f, 0.5f, 0.5f, 1.0f): (i % 2 == 0 ? glm::vec4(0.9f, 0.9f, 0.9f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

				int index = 0;
				for (size_t i = 0; i < radialMenu->menuItems.size(); i++)
				{
					if (!radialMenu->menuItems[i]->enabled)
						continue;
					glm::mat4 matrix;
					matrix = glm::translate(matrix, glm::vec3(radialMenuPosition, 0));
					matrix = glm::rotate(matrix, index * 45.0f, glm::vec3(0, 0, 1));
					matrix = glm::translate(matrix, glm::vec3(-120, 0, 0));
					matrix = glm::scale(matrix, glm::vec3(0.75f, 0.75f, 1.0f));
					matrix = glm::translate(matrix, glm::vec3(0, -16, 0));


					ToggleMenuItem* toggle = dynamic_cast<ToggleMenuItem*>(radialMenu->menuItems[i]);
					if (toggle)
					{
						spriteBatch.draw(radialmenufont, (toggle->getValue() ? "[x]  " : "[  ]  ") + radialMenu->menuItems[i]->title, matrix, glm::vec4(0, 0, 0, 1));
					}
					else
						spriteBatch.draw(radialmenufont, radialMenu->menuItems[i]->title, matrix, glm::vec4(0, 0, 0, 1));
					index++;

				}
			}

			if (menuBar)
			{
				spriteBatch.drawStretchyRect(skinTexture, glm::mat4(), skin["list"], glm::vec2(screenSize.x, 18));
				float posX = 10;
				int posY = 2;

				for (size_t i = 0; i < menuBar->menuItems.size(); i++)
				{
					SubMenuMenuItem* item = dynamic_cast<SubMenuMenuItem*>(menuBar->menuItems[i]);
					if (!item->enabled)
						continue;
					float len = font->textlen(menuBar->menuItems[i]->title);

					if (item && item->opened)
						spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(glm::vec2(posX-2, posY)), skin["list"], glm::vec2(len + 40, 16), glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));

					if (mouseState.position.x > posX && mouseState.position.x <= posX + len + 40 && mouseState.position.y > posY && mouseState.position.y < posY+16)
						spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(glm::vec2(posX - 2, posY)), skin["list"], glm::vec2(len + 40, 16), glm::vec4(0.5f, 0.5f, 0.9f, 1.0f));

					spriteBatch.draw(font, menuBar->menuItems[i]->title, blib::math::easyMatrix(glm::vec2(posX, posY)), glm::vec4(0, 0, 0, 1));
					posX += len + 40;
				}
				
				for(size_t i = 0; i < popupMenus.size(); i++)
				{
					std::pair<glm::vec2, Menu*> &item = popupMenus[i];
					
					spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(item.first), skin["list"], glm::vec2(200, 16 * item.second->menuItems.size())); //TODO: calculate enaebledCount
					int iii = 0;
					for (size_t ii = 0; ii < item.second->menuItems.size(); ii++)
					{
						if (!item.second->menuItems[ii]->enabled)
							continue;
						if (mouseState.position.x > item.first.x && mouseState.position.x < item.first.x + 200 && mouseState.position.y > item.first.y + 16 * iii && mouseState.position.y < item.first.y + 16 + 16 * iii)
							spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(glm::vec2(item.first.x - 2, item.first.y + 16*iii)), skin["list"], glm::vec2(200, 16), glm::vec4(0.5f, 0.5f, 0.9f, 1.0f));


						spriteBatch.draw(font, item.second->menuItems[iii]->title, blib::math::easyMatrix(glm::vec2(item.first.x + 2, item.first.y + 2 + 16 * iii)), glm::vec4(0, 0, 0, 1));
						iii++;
					}
				}
			}
			if (popupMenu)
			{
				spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(popupMenuPosition), skin["list"], glm::vec2(200, 16 * popupMenu->menuItems.size())); //TODO: calculate enaebledCount
				int iii = 0;
				for (size_t ii = 0; ii < popupMenu->menuItems.size(); ii++)
				{
					if (!popupMenu->menuItems[ii]->enabled)
						continue;
					if (mouseState.position.x > popupMenuPosition.x && mouseState.position.x < popupMenuPosition.x + 200 && mouseState.position.y > popupMenuPosition.y + 16 * iii && mouseState.position.y < popupMenuPosition.y + 16 + 16 * iii)
						spriteBatch.drawStretchyRect(skinTexture, blib::math::easyMatrix(glm::vec2(popupMenuPosition.x - 2, popupMenuPosition.y + 16 * iii)), skin["list"], glm::vec2(200, 16), glm::vec4(0.5f, 0.5f, 0.9f, 1.0f));


					spriteBatch.draw(font, popupMenu->menuItems[iii]->title, blib::math::easyMatrix(glm::vec2(popupMenuPosition.x + 2, popupMenuPosition.y + 2 + 16 * iii)), glm::vec4(0, 0, 0, 1));
					iii++;
				}
			}

			prevMouseState = mouseState; // TODO: move this to an update method?
		}

		void WM::setSkin( std::string skinFile, ResourceManager* resourceManager )
		{
			skin = util::FileSystem::getJson(skinFile);
			if (skin.isMember("texture"))
				skinTexture = resourceManager->getResource<Texture>(skin["texture"].asString());
			font = resourceManager->getResource<Font>("tahoma");
			if (skin.isMember("radialfont"))
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

		void WM::addTask( const std::function<void()> & task )
		{
			tasks.push_back(task);
		}

		blib::wm::Menu* WM::loadMenu( std::string filename, const blib::json::Value &translation )
		{
			json::Value menuData = blib::util::FileSystem::getJson(filename);

			Menu* root = new Menu(menuData);

			for (auto it = translation.begin(); it != translation.end(); it++)
			{
				auto item = root->getItem(it.key());
				if(item)
					item->title = it.value().asString()	;
			}
			


			root->foreach([this](MenuItem* item) {
				if (item->key != Key::NONE)
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
			mouseState.position.x = x; 
			mouseState.position.y = y; 
			mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = true; 


			if (radialMenu)
			{
				glm::vec2 diff = radialMenuPosition - glm::vec2(mouseState.position);
				float angle = atan2(diff.y, diff.x);
                int id = (int)glm::round(((angle + 2 * blib::math::pif) / (2 * blib::math::pif)) * 8) % 8;
				for (size_t i = 0; (int)i <= id && i < radialMenu->menuItems.size(); i++)
					if (!radialMenu->menuItems[i]->enabled)
						id++;

				if (id < (int)radialMenu->menuItems.size())
				{
					SubMenuMenuItem* submenuitem = dynamic_cast<SubMenuMenuItem*>(radialMenu->menuItems[id]);
					if (submenuitem) 
					{
						radialMenu = submenuitem->menu;
						radialMenuPosition = glm::vec2(mouseState.position);
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

			if (menuBar)
			{
				if (y < 16)
				{
					menuBarOpen = !menuBarOpen;
					popupMenus.clear();
					if (menuBarOpen)
					{
						float posX = 10;
						for (size_t i = 0; i < menuBar->menuItems.size(); i++)
						{
							SubMenuMenuItem* item = dynamic_cast<SubMenuMenuItem*>(menuBar->menuItems[i]);
							if (!item->enabled)
								continue;
							float len = font->textlen(menuBar->menuItems[i]->title);
							if (mouseState.position.x > posX && mouseState.position.x <= posX + len + 40 && mouseState.position.y > 0 && mouseState.position.y < 0 + 16)
								popupMenus.push_back(std::pair<glm::vec2, Menu*>(glm::vec2(posX - 10, 18), ((SubMenuMenuItem*)menuBar->menuItems[i])->menu));
							posX += len + 40;
						}
					}
					return true;
				}
			}
			for (size_t i = 0; i < popupMenus.size(); i++)
			{
				std::pair<glm::vec2, Menu*>& item = popupMenus[i];

				if (x > item.first.x && x < item.first.x + 200 &&
					y > item.first.y && y < item.first.y + 16 * item.second->menuItems.size())
				{
					int selected = (int)glm::floor(y - item.first.y) / 16;
					SubMenuMenuItem* subMenu = dynamic_cast<SubMenuMenuItem*>(item.second->menuItems[selected]);
					if (subMenu)
					{
						popupMenus.push_back(std::pair<glm::vec2, Menu*>(glm::vec2(item.first.x + 200, item.first.y + 12 * selected), subMenu->menu));
					}
					ActionMenuItem* actionMenu = dynamic_cast<ActionMenuItem*>(item.second->menuItems[selected]);
					if (actionMenu)
					{
						menuBarOpen = false;
						popupMenus.clear();
						if (actionMenu->callback)
							actionMenu->callback();
						return true;
					}

				}
			}
			if (popupMenu)
			{
				if (x > popupMenuPosition.x && x < popupMenuPosition.x + 200 &&
					y > popupMenuPosition.y && y < popupMenuPosition.y + 16 * popupMenu->menuItems.size())
				{
					int selected = (int)glm::floor(y - popupMenuPosition.y) / 16;
					SubMenuMenuItem* subMenu = dynamic_cast<SubMenuMenuItem*>(popupMenu->menuItems[selected]);
					if (subMenu)
					{
						popupMenus.push_back(std::pair<glm::vec2, Menu*>(glm::vec2(popupMenuPosition.x + 200, popupMenuPosition.y + 12 * selected), subMenu->menu));
					}
					ActionMenuItem* actionMenu = dynamic_cast<ActionMenuItem*>(popupMenu->menuItems[selected]);
					if (actionMenu)
					{
						menuBarOpen = false;
						popupMenus.clear();
						if (actionMenu->callback)
							actionMenu->callback();
						delete popupMenu;
						popupMenu = NULL;
						return true;
					}

				}
			}




			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if (w->inWindow(x, y) && w->visible)
				{
					if (windows.front() != w && windows.front()->selectedWidget) //unselect stuff
					{
						windows.front()->selectedWidget->selected = false;
						windows.front()->selectedWidget = NULL;
					}

					windows.erase(it); // move window to top
					windows.push_front(w);

					bool ret = false;
					clickX = x;
					clickY = y;
					if (w->inComponent(x, y))
					{
						ret = w->onMouseDown(x, y, clickCount);
					}
					else
						draggingWindow = w;
					return ret;
				}
			}
			return false;
		}
		bool WM::onMouseUp(int x, int y, Button button, int clickCount)
{
			mouseState.position.x = x; 
			mouseState.position.y = y; 
			mouseState.buttons[button == MouseListener::Left ? 0 : (button == MouseListener::Middle ? 1 : 2)] = false; 


			if (draggingWindow != NULL)
				draggingWindow = NULL;


			if (menuBar && y < 16) //ignore clicks on menubar
					return true;

			for (size_t i = 0; i < popupMenus.size(); i++)
			{
				std::pair<glm::vec2, Menu*>& item = popupMenus[i];

				if (x > item.first.x && x < item.first.x + 200 &&
					y > item.first.y && y < item.first.y + 16 * item.second->menuItems.size())
					return true;
			}


			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				Window* w = (*it);
				if (w->inWindow(x, y) && w->visible)
				{
					w->onMouseUp(x, y, clickCount);
					if (abs(clickX - x) < 3 && abs(clickY - y) < 3)
					{
						w->onMouseClick(x, y, clickCount);
					}
					return true;
				}
			}
			return false;
		}
		
		bool WM::onMouseMove(int x, int y, Buttons button)
{
			mouseState.position.x = x; 
			mouseState.position.y = y; 

			bool handled = false;

			if (menuBarOpen)
			{
				if (y < 16)
				{
					popupMenus.clear();
					float posX = 10;
					for (size_t i = 0; i < menuBar->menuItems.size(); i++)
					{
						SubMenuMenuItem* item = dynamic_cast<SubMenuMenuItem*>(menuBar->menuItems[i]);
						if (!item->enabled)
							continue;
						float len = font->textlen(menuBar->menuItems[i]->title);
						if (mouseState.position.x > posX && mouseState.position.x <= posX + len + 40 && mouseState.position.y > 0 && mouseState.position.y < 0 + 16)
							popupMenus.push_back(std::pair<glm::vec2, Menu*>(glm::vec2(posX-10, 18), ((SubMenuMenuItem*)menuBar->menuItems[i])->menu));
						posX += len + 40;
					}
				}



			}


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
						if (w->closable && dragMode == (2 | 4))
							newCursor = ARROW;
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
					draggingWindow->x += x - prevMouseState.position.x;
					draggingWindow->y += y - prevMouseState.position.y;
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
					if (w->inWindow(x, y) && w->visible && w->selectedWidget)
					{
						w->selectedWidget->onDrag(
							x - w->x - w->selectedWidget->absoluteX() - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), 
							y - w->y - w->selectedWidget->absoluteY() - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
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
				if (w->inWindow(mouseState.position.x, mouseState.position.y) && w->visible)
				{
					w->onScroll(mouseState.position.x, mouseState.position.y, delta);
					return true;
				}
				if (it == windows.begin() && w->modal)
					return true;
			}
			return false;
		}

		bool WM::onKeyDown(Key key)
		{

			if (!windows.empty())
			{
				if (windows.front()->onKeyDown(key))
					return true;
				if (windows.front()->selectedWidget && windows.front()->selectedWidget->canHaveKeyboardFocus)
				{
					if (windows.front()->selectedWidget->onKeyDown(key))
						keyPressed = true;
					return true;
				}
				if (windows.front()->defaultWidget)
				{
					if (windows.front()->defaultWidget->onKeyDown(key))
						keyPressed = true;
					return true;
				}
			}

			if (menuKeys.find(key) != menuKeys.end())
			{
				bool ret = false;
				{
					ToggleMenuItem* item = dynamic_cast<ToggleMenuItem*>(menuKeys[key]);
					if (item)
					{
						item->toggle();
						ret = true;
					}
				}

				{
					ActionMenuItem* item = dynamic_cast<ActionMenuItem*>(menuKeys[key]);
					if (item)
					{
						if (item->callback)
						{
							item->callback();
							ret = true;
						}
					}
				}
				if (ret)
					return true;
			}

			if (key == Key::SPACE)
			{
				radialMenu = radialMenu ? NULL : radialMenuRoot;
				radialMenuPosition.x = (float)mouseState.position.x;
				radialMenuPosition.y = (float)mouseState.position.y;
			}

			return false;
		}

		bool WM::onKeyUp(Key key)
		{
			if (!windows.empty())
			{
				if (windows.front()->onKeyUp(key))
					return true;
				if (windows.front()->selectedWidget && windows.front()->selectedWidget->canHaveKeyboardFocus)
					if (windows.front()->selectedWidget->onKeyUp(key))
					{
						keyPressed = true;
						return true;
					}
			}
			return false;
		}

		bool WM::onChar(char character)
		{
			if (!windows.empty())
			{
				if (windows.front()->onChar(character))
					return true;
				if (windows.front()->selectedWidget && windows.front()->selectedWidget->canHaveKeyboardFocus)
					if (windows.front()->selectedWidget->onChar(character))
					{
						keyPressed = true;
						return true;
					}
			}
			return false;
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
			if (radialMenu)
				return true;
			if (menuBar && y < 16)
				return true;
			for (size_t i = 0; i < popupMenus.size(); i++)
			{
				std::pair<glm::vec2, Menu*>& item = popupMenus[i];

				if (x > item.first.x && x < item.first.x + 200 &&
					y > item.first.y && y < item.first.y + 16 * item.second->menuItems.size())
					return true;
			}


			for (std::list<Window*>::iterator it = windows.begin(); it != windows.end(); it++)
			{
				if ((*it)->inWindow(x, y) && (*it)->visible)
					return true;
			}
			return false;
		}

		void WM::setMenuBar(blib::wm::Menu* menu)
		{
			menuBar = menu;
		}

		void WM::setPopupMenuPosition(const glm::vec2 &pos)
		{
			this->popupMenuPosition = pos;
			if (popupMenuPosition.x + 200 > screenSize.x)
				popupMenuPosition.x = (float)(screenSize.x - 200);
		}


	}
}

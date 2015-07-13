#include "Window.h"

#include <algorithm>
#include <cctype>


#include <blib/config.h>

#if defined(BLIB_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(BLIB_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <blib/wm/WM.h>
#include <blib/Texture.h>
#include <blib/ResourceManager.h>
#include <blib/SpriteBatch.h>
#include <blib/util/Log.h>
#include <blib/util/FileSystem.h>
#include <blib/math/Rectangle.h>
#include <blib/json.h>

#include <blib/wm/widgets/Panel.h>
#include <blib/wm/widgets/button.h>
#include <blib/wm/widgets/textbox.h>
#include <blib/wm/widgets/Label.h>
#include <blib/wm/widgets/list.h>
#include <blib/wm/widgets/ScrollPanel.h>
#include <blib/wm/widgets/Image.h>
#include <blib/wm/widgets/BmlBox.h>
#include <blib/wm/widgets/TreeView.h>

using blib::util::Log;


namespace blib
{
	namespace wm
	{

		Window::Window(std::string title, std::string skinFile, ResourceManager* resourceManager)
		{
			this->title = title;
			json::Value skin = util::FileSystem::getJson("assets/windows/" + skinFile);

			this->x = 100;
			this->y = 100;
			this->movable = true;
			this->enabled = true;
			this->visible = true;
			this->rootPanel = NULL;
			if (skinFile != "")
			{
				this->setSize(skin["size"][0].asInt(), skin["size"][1].asInt());
				if (skin.isMember("modal"))
					this->modal = skin["modal"].asBool();
				else
					this->modal = false;
				this->resizable = skin["resizable"].asBool();
			}
			this->rootPanel = new widgets::Panel();
			this->rootPanel->x = 0;
			this->rootPanel->y = 0;
			this->rootPanel->width = skin["size"][0].asInt();
			this->rootPanel->height = skin["size"][1].asInt();

			defaultWidget = NULL;
			selectedWidget = NULL;

			if (skinFile != "")
				this->addWidgets(this->rootPanel, skin["widgets"], resourceManager);

			if (skin.isMember("closable") && skin["closable"].asBool())
				closable = true;
			else
				closable = false;

			if (skin.isMember("defaultwidget"))
				defaultWidget = getComponent(skin["defaultwidget"]);

		/*	addKeyDownHandler([this](blib::Key key) { 
				if (selectedWidget)
					return selectedWidget->onKeyDown(key);
				return false;
			});
			addKeyUpHandler([this](blib::Key key) { 
				if (selectedWidget)
					return selectedWidget->onKeyUp(key);
				return false;
			});
			addCharHandler([this](char key) { 
				if (selectedWidget)
					return selectedWidget->onChar(key);
				return false;
			});*/


			addClickHandler([this](int x, int y, int clickcount) 
			{ 
				if (closable && y-this->y < WM::getInstance()->skin["window"]["offsets"]["top"].asInt() && x-this->x > width - WM::getInstance()->skin["window"]["offsets"]["top"].asInt())
					close();
				return rootPanel->onMouseClick(x - this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y - this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt(), clickcount);
			});
			addMouseUpHandler([this](int x, int y, int clickcount) { return rootPanel->onMouseUp(x - this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y - this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt(), clickcount); });
			addMouseDownHandler([this](int x, int y, int clickcount) { 
				if (selectedWidget)
					selectedWidget->selected = false;
				selectedWidget = rootPanel->getComponent(x - this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y - this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
				if (selectedWidget)
					selectedWidget->selected = true;

				return rootPanel->onMouseDown(x - this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y - this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt(), clickcount); });
			addScrollHandler([this](int x, int y, int delta) { 
				return rootPanel->onScroll(
					x - this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), 
					y - this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt(), delta); });



			WM::getInstance()->addWindow(this);
		}

		void Window::setSize( int width, int height )
		{
			this->width = width + WM::getInstance()->skin["window"]["offsets"]["left"].asInt() + WM::getInstance()->skin["window"]["offsets"]["right"].asInt();
			this->height = height + WM::getInstance()->skin["window"]["offsets"]["top"].asInt() + WM::getInstance()->skin["window"]["offsets"]["bottom"].asInt();
			if(rootPanel)
			{
				int oldWidth = rootPanel->width;
				int oldHeight = rootPanel->height;
				rootPanel->width = width;
				rootPanel->height = height;
				rootPanel->arrangeComponents(oldWidth, oldHeight);
			}
		}

		void Window::center()
		{
		//	this->x = (BrowEdit::getInstance()->width-width)/2;
		//	this->y = (BrowEdit::getInstance()->height-height)/2;
		}

		void Window::close()
		{
			WM::getInstance()->removeWindow(this);
		}

		void Window::show()
		{
			setVisible(true);
		}
		void Window::hide()
		{
			setVisible(false);
		}

		void Window::draw(SpriteBatch &spriteBatch, Renderer* renderer)
		{
			glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(x,y,0));
			spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, matrix, WM::getInstance()->skin["window"], glm::vec2(width,height));

			if (closable)
				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, blib::math::easyMatrix(glm::vec2(width-13, 3), matrix), WM::getInstance()->skin["closebutton"], glm::vec2(10, 10));

			spriteBatch.draw(WM::getInstance()->font, title, glm::translate(glm::mat4(), glm::vec3(x+20, y+1,0)), glm::vec4(0,0,0,1));
			rootPanel->draw(spriteBatch, glm::translate(matrix, glm::vec3(WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), WM::getInstance()->skin["window"]["offsets"]["top"].asInt(),0)), renderer);
		}

		void Window::addWidgets( widgets::Panel* panel, json::Value skin, ResourceManager* resourceManager )
		{
			
			for (auto it = skin.begin(); it != skin.end(); it++)
			{
				json::Value widgetSkin = it.value();
				Widget* widget = NULL;
				std::string type = widgetSkin["type"].asString();
				if(type == "button")
				{
					widget = new widgets::Button(widgetSkin["text"].asString());
				}
				else if(type == "textbox")
				{
					widget = new widgets::Textbox();
					if(widgetSkin.isMember("emptytext"))
						((widgets::Textbox*)widget)->emptyText = widgetSkin["emptytext"].asString();
					if(widgetSkin.isMember("text"))
						((widgets::Textbox*)widget)->text = widgetSkin["text"].asString();
			
				}
				else if (type == "list")
					widget = new widgets::List();
				else if (type == "tree")
					widget = new widgets::TreeView();
				else if (type == "label")
				{
					widget = new widgets::Label();
					if(widgetSkin.isMember("text"))
						((widgets::Label*)widget)->text = widgetSkin["text"].asString();
				}
				else if(type == "scrollpanel")
					widget = new widgets::ScrollPanel();
				else if (type == "image")
					widget = new widgets::Image(resourceManager->getResource<blib::Texture>(widgetSkin["src"].asString()));
				else if (type == "bmlbox")
					widget = new widgets::BmlBox(resourceManager);
				else
				{
					Log::out<<"Unknown widget type: "<<type<<Log::newline;
					continue;
				}

				widget->name = it.key();

				if(widgetSkin["position"][0].asInt() != -1)
					widget->x = widgetSkin["position"][0].asInt();
				if(widgetSkin["position"][1].asInt() != -1)
					widget->y = widgetSkin["position"][1].asInt();
				if(widgetSkin["size"][0].asInt() != -1)
					widget->width = widgetSkin["size"][0].asInt();
				if(widgetSkin["size"][1].asInt() != -1)
					widget->height = widgetSkin["size"][1].asInt();

				widget->parent = panel;
				widget->positionHelpLeft =	toPositionHelp(widgetSkin["positionhelp"]["left"].asString());
				widget->positionHelpRight = toPositionHelp(widgetSkin["positionhelp"]["right"].asString());
				widget->positionHelpTop =	toPositionHelp(widgetSkin["positionhelp"]["top"].asString());
				widget->positionHelpBottom =toPositionHelp(widgetSkin["positionhelp"]["bottom"].asString());


				panel->add(widget);
			}
		}








		Widget::PositionHelp Window::toPositionHelp( std::string text )
		{
			std::transform(text.begin(), text.end(), text.begin(), ::tolower);
			if(text == "left")
				return Widget::LEFT;
			else if(text == "right")
				return Widget::RIGHT;
			else if(text == "bottom")
				return Widget::BOTTOM;
			else if(text == "top")
				return Widget::TOP;
			return Widget::NOTHING;
		}

		bool Window::inWindow( int x, int y )
		{
			return x >= this->x && y >= this->y && x < this->x + this->width && y < this->y + this->height;
		}

		void Window::arrangeComponents( int oldWidth, int oldHeight )
		{
			int oldPanelWidth = rootPanel->width;
			int oldPanelHeight = rootPanel->height;
			rootPanel->width = width - WM::getInstance()->skin["window"]["offsets"]["left"].asInt() - WM::getInstance()->skin["window"]["offsets"]["right"].asInt();
			rootPanel->height = height - WM::getInstance()->skin["window"]["offsets"]["top"].asInt() - WM::getInstance()->skin["window"]["offsets"]["bottom"].asInt();
			rootPanel->arrangeComponents(oldPanelWidth, oldPanelHeight);
		}

		/*void Window::mousewheel( int direction, int x, int y )
		{
			rootPanel->mousewheel(direction, x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}*/

		bool Window::inComponent( int x, int y )
		{
			return rootPanel->inComponent(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		Widget* Window::getComponent( int x, int y )
		{
			return rootPanel->getComponent(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		Widget* Window::getComponent( std::string name )
		{
			return rootPanel->getComponent(name);
		}

		void Window::setPosition(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		void Window::setVisible(bool visible)
		{
			this->visible = visible;
		}

	}
}

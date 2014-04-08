#include "Window.h"

#include <json/value.h>

#include <algorithm>
#include <cctype>

#ifdef ANDROID
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
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

#include <blib/wm/widgets/Panel.h>
#include <blib/wm/widgets/button.h>
#include <blib/wm/widgets/textbox.h>
#include <blib/wm/widgets/Label.h>
#include <blib/wm/widgets/list.h>
#include <blib/wm/widgets/ScrollPanel.h>
#include <blib/wm/widgets/Image.h>

using blib::util::Log;


namespace blib
{
	namespace wm
	{

		Window::Window( std::string title, std::string skinFile, ResourceManager* resourceManager )
		{
			this->title = title;
			Json::Value skin = util::FileSystem::getJson("assets/windows/" + skinFile);
	
			this->x = 100;
			this->y = 100;
			this->movable = true;
			this->enabled = true;
			this->visible = true;
			this->rootPanel = NULL;
			if(skinFile != "")
			{
				this->setSize(skin["size"][0u].asInt(),skin["size"][1u].asInt());
				this->modal = skin["modal"].asBool();
				this->resizable = skin["resizable"].asBool();
			}

			this->rootPanel = new widgets::Panel();
			this->rootPanel->x = 0;
			this->rootPanel->y = 0;
			this->rootPanel->width = skin["size"][0u].asInt();
			this->rootPanel->height = skin["size"][1u].asInt();

			selectedWidget = NULL;

			if(skinFile != "")
				this->addWidgets(this->rootPanel, skin["widgets"], resourceManager);

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

		void Window::draw( SpriteBatch &spriteBatch )
		{
			glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(x,y,0));
			spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, matrix, WM::getInstance()->skin["window"], glm::vec2(width,height));
			spriteBatch.draw(WM::getInstance()->font, title, glm::translate(glm::mat4(), glm::vec3(x+20, y+1,0)), glm::vec4(0,0,0,1));
			rootPanel->draw(spriteBatch, glm::translate(matrix, glm::vec3(WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), WM::getInstance()->skin["window"]["offsets"]["top"].asInt(),0)));
		}

		void Window::addWidgets( widgets::Panel* panel, Json::Value skin, ResourceManager* resourceManager )
		{
			Json::Value::Members members = skin.getMemberNames();
			for(unsigned int i = 0; i < members.size(); i++)
			{
				Json::Value widgetSkin = skin[members[i]];
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
				else if(type == "list")
					widget = new widgets::List();
				else if(type == "label")
				{
					widget = new widgets::Label();
					if(widgetSkin.isMember("text"))
						((widgets::Label*)widget)->text = widgetSkin["text"].asString();
				}
				else if(type == "scrollpanel")
					widget = new widgets::ScrollPanel();
				else if(type == "image")
					widget = new widgets::Image(resourceManager->getResource<blib::Texture>(widgetSkin["src"].asString()));
				else
				{
					Log::out<<"Unknown widget type: "<<type<<Log::newline;
					continue;
				}

				widget->name = members[i];

				if(widgetSkin["position"][0u].asInt() != -1)
					widget->x = widgetSkin["position"][0u].asInt();
				if(widgetSkin["position"][1u].asInt() != -1)
					widget->y = widgetSkin["position"][1u].asInt();
				if(widgetSkin["size"][0u].asInt() != -1)
					widget->width = widgetSkin["size"][0u].asInt();
				if(widgetSkin["size"][1u].asInt() != -1)
					widget->height = widgetSkin["size"][1u].asInt();

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

		void Window::mousewheel( int direction, int x, int y )
		{
			rootPanel->mousewheel(direction, x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		bool Window::inComponent( int x, int y )
		{
			return rootPanel->inComponent(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		Widget* Window::getComponent( int x, int y )
		{
			return rootPanel->getComponent(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}


		void Window::mousedown(int x, int y)
		{
			rootPanel->mousedown(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		void Window::mousedrag(int x, int y)
		{
			rootPanel->mousedrag(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		void Window::mouseup(int x, int y)
		{
			rootPanel->mouseup(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
		}

		void Window::mouseclick(int x, int y, int clickcount)
		{
			if(selectedWidget)
				selectedWidget->selected = false;
			selectedWidget = rootPanel->getComponent(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt());
			if(selectedWidget)
				selectedWidget->selected = true;
			rootPanel->mouseclick(x-this->x - WM::getInstance()->skin["window"]["offsets"]["left"].asInt(), y-this->y - WM::getInstance()->skin["window"]["offsets"]["top"].asInt(), clickcount);
		}

		void Window::keyboard(char key)
		{
			if(selectedWidget)
				selectedWidget->keyboard(key);
		}

		void Window::keyboardSpecial(int key)
		{
			if(selectedWidget)
				selectedWidget->keyboardSpecial(key);
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

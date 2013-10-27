#pragma once

#include <string>
#include <json/value.h>
#include <blib/wm/widgets/Widget.h>
#include <blib/wm/widgets/Panel.h>

namespace blib
{
	class ResourceManager;
	namespace wm
	{

	class Window
	{
		friend class WM;
		void addWidgets( widgets::Panel* panel, Json::Value skin, ResourceManager* resourceManager );

	protected:
		std::string title;

		int x;
		int y;
		int width;
		int height;

		bool enabled;
		bool modal;
		bool resizable;
		widgets::Panel* rootPanel;
		bool movable;
		bool visible;

		Widget* selectedWidget;

	public:
		Window(std::string title, std::string skinFile, ResourceManager* resourceManager);

		void setSize(int width, int height);
		void center();
		void close();

		void show();
		void hide();

		virtual void draw(SpriteBatch &spriteBatch);
		Widget::PositionHelp toPositionHelp( std::string text );
		bool inWindow( int x, int y );
		void arrangeComponents( int oldWidth, int oldHeight );


		void mousewheel(int direction, int x, int y);
		void mousedown(int x, int y);
		void mouseup(int x, int y);
		void mouseclick(int x, int y, int clickcount);
		void mousedrag(int x, int y);
		void keyboard(char key);
		void keyboardSpecial(int key);


		template<class T>
		T* getComponent(std::string name)
		{
			return (T*)rootPanel->getComponent(name);
		}

		Widget* getComponent(std::string name);
		Widget* getComponent(int x, int y);
		bool inComponent( int x, int y );

		friend class Widget;
	};
	}
}

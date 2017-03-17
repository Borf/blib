#pragma once

#include <string>
#include <blib/json.hpp>
#include <blib/wm/widgets/Widget.h>
#include <blib/wm/widgets/Panel.h>
#include <blib/wm/KeyboardFocusable.h>
#include <blib/wm/MouseClickable.h>

namespace blib
{
	class ResourceManager;
	namespace wm
	{

	class Window : public KeyboardFocusable, MouseClickable
	{
		friend class WM;
		void addWidgets( widgets::Panel* panel, json skin, ResourceManager* resourceManager );

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
		bool closable;

		Widget* selectedWidget;
		Widget* defaultWidget;

	public:
		Window(std::string title, std::string skinFile, ResourceManager* resourceManager);

		void setSize(int width, int height);
		inline int getWidth() const { return width; }
		inline int getHeight() const { return height; }

		void setPosition(int x, int y);

		void center();
		void close();

		void show();
		void hide();
		void setVisible(bool visible);

		virtual void draw(SpriteBatch &spriteBatch, Renderer* renderer);
		Widget::PositionHelp toPositionHelp( std::string text );
		bool inWindow( int x, int y );
		virtual void arrangeComponents( int oldWidth, int oldHeight );

		template<class T>
		T* getComponent(std::string name) const
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

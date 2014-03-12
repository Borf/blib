#pragma once

#include <list>

#include <blib/util/Singleton.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <json/value.h>
#include <glm/glm.hpp>
#include <blib/util/FastDelegate.h>

namespace blib
{
	class Font; 
	class Texture; 
	class SpriteBatch;
	class ResourceManager;

	namespace wm
	{
		class Window;
		class Widget;
		class Menu;


		class WM : public util::Singleton<WM>, public MouseListener, public KeyListener
		{
			std::list<Window*> windows;

			int clickX, clickY, clickTime;
			Widget* clickWidget;
			Widget* hoverWidget;

			std::list<fastdelegate::FastDelegate0<> > tasks;

		public:
			WM();
			~WM();

			void addWindow(Window* window);
			void removeWindow(Window* window);
			void setSkin(std::string skinFile, ResourceManager* resourceManager);
			void setFont(Font* font);

			void addTask(fastdelegate::FastDelegate0<> task);
			void draw(SpriteBatch &spriteBatch);
			bool mousedown(int x, int y);
			bool mouseup(int x, int y, int clickcount);
			bool mousemove(int oldX, int oldY, int x, int y );
			bool mousewheel( int direction, int x, int y );
			bool keyboard(char key);
			bool keyboardSpecial(int key);
			bool hasModalWindow();
			glm::vec4 convertHexColor4( std::string hexColor );
			blib::wm::Menu* loadMenu( std::string filename );
			void setRadialMenu( blib::wm::Menu* param1 );

			virtual void onMouseDown(int x, int y, Button button, int clickCount);
			virtual void onMouseUp(int x, int y, Button button, int clickCount);
			virtual void onMouseMove(int x, int y, Buttons button);
			virtual void onScroll(int delta);
			virtual void onKeyDown(Key key);
			virtual void onKeyUp(Key key);
			virtual void onChar(char character);

			Json::Value skin;
			Texture* skinTexture;
			Font* font;

			Window* draggingWindow;
			int		dragMode;

			bool	leftbuttondown;

			enum Cursor
			{
				ARROW,
				WAIT,
				RESIZE_H,
				RESIZE_V,
				RESIZE_TL,
				RESIZE_BL,
			} currentCursor;


			friend class Widget;

		};
	}
}


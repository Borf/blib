#pragma once

#include <list>

#include <blib/util/Singleton.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/App.h> //for mousestate
#include <json/value.h>
#include <glm/glm.hpp>
#include <blib/util/FastDelegate.h>
#include <blib/gl/GlResizeRegister.h>

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


		class WM : public util::Singleton<WM>, public MouseListener, public KeyListener, blib::gl::GlResizeRegister
		{
			glm::vec2 screenSize;
			MouseState mouseState;
			MouseState prevMouseState;
			std::list<Window*> windows;

			int clickX, clickY, clickTime;
			Widget* clickWidget;
			Widget* hoverWidget;

			std::list<fastdelegate::FastDelegate0<> > tasks;

			blib::wm::Menu* radialMenuRoot;
			blib::wm::Menu* radialMenu;
			blib::wm::Menu* pulldownMenu;
			blib::wm::Menu* popupMenu;

		public:
			WM();
			~WM();

			void addWindow(Window* window);
			void removeWindow(Window* window);
			void center(Window* window);

			void setSkin(std::string skinFile, ResourceManager* resourceManager);
			void setFont(Font* font);

			void addTask(fastdelegate::FastDelegate0<> task);
			void draw(SpriteBatch &spriteBatch);
			bool keyboard(char key);
			bool keyboardSpecial(int key);
			bool hasModalWindow();
			glm::vec4 convertHexColor4( std::string hexColor );
			blib::wm::Menu* loadMenu( std::string filename );
			void setRadialMenu( blib::wm::Menu* param1 );

			virtual bool onMouseDown(int x, int y, Button button, int clickCount);
			virtual bool onMouseUp(int x, int y, Button button, int clickCount);
			virtual bool onMouseMove(int x, int y, Buttons button);
			virtual bool onScroll(int delta);
			virtual void onKeyDown(Key key);
			virtual void onKeyUp(Key key);
			virtual void onChar(char character);

			virtual void resizeGl(int width, int height);

			Json::Value skin;
			Texture* skinTexture;
			Font* font;

			Window* draggingWindow;
			int		dragMode;

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


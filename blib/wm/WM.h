#pragma once

#include <list>
#include <map>

#include <blib/util/Singleton.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/App.h> //for mousestate
#include <blib/json.hpp>
#include <glm/glm.hpp>
#include <blib/gl/GlResizeRegister.h>
#include <functional>

namespace blib
{
	class Font; 
	class Texture; 
	class SpriteBatch;
	class ResourceManager;
	enum class Key;

	class ToolBar;

	namespace wm
	{
		class Window;
		class Widget;
		class Menu;
		class MenuItem;



		class WM : public util::Singleton<WM>, public MouseListener, public KeyListener, blib::gl::GlResizeRegister
		{
			MouseState mouseState;
			MouseState prevMouseState;
			std::list<Window*> windows;

			int clickX, clickY, clickTime;
			Widget* clickWidget;
			Widget* hoverWidget;

			std::list<std::function<void()> > tasks;

			blib::wm::Menu* radialMenuRoot;
			blib::wm::Menu* radialMenu;
			glm::vec2		radialMenuPosition;
			double			radialMenuAnimation = 0;


			blib::wm::Menu* menuBar;
			bool menuBarOpen;
			glm::vec2		popupMenuPosition;

			std::vector<std::pair<glm::vec2, blib::wm::Menu*> > popupMenus;

			std::map<Key, MenuItem*> menuKeys;

			ToolBar* toolBar = nullptr;


		public:
			blib::wm::Menu* popupMenu;
			void setPopupMenuPosition(const glm::vec2 &pos);

			WM();
			~WM();

			void addWindow(Window* window);
			void removeWindow(Window* window);
			void center(Window* window);

			void setSkin(std::string skinFile, ResourceManager* resourceManager);
			void setFont(Font* font);

			void addTask(const std::function<void()> &task);
			void draw(SpriteBatch &spriteBatch, Renderer* renderer);
			bool keyboard(char key);
			bool keyboardSpecial(int key);
			bool hasModalWindow();
			glm::vec4 convertHexColor4( std::string hexColor );
			blib::wm::Menu* loadMenu( std::string filename, const json &translation);
			void setRadialMenu( blib::wm::Menu* param1 );
			void setMenuBar(blib::wm::Menu* menu);

			//from listeners
			virtual bool onMouseDown(int x, int y, Button button, int clickCount);
			virtual bool onMouseUp(int x, int y, Button button, int clickCount);
			virtual bool onMouseMove(int x, int y, Buttons button);
			virtual bool onScroll(int delta);
			virtual bool onKeyDown(Key key);
			virtual bool onKeyUp(Key key);
			virtual bool onChar(char character);

			virtual void resizeGl(int width, int height, int offx, int offy);

			bool inWindow(const glm::ivec2 &pos) { return inWindow(pos.x, pos.y);  }
			bool inWindow(int x, int y);
			json skin;
			Texture* skinTexture;
			Font* font;
			Font* radialmenufont;

			Window* draggingWindow;
			int		dragMode;
			bool keyPressed;

			glm::ivec2 screenSize;

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


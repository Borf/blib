#pragma once

#include <list>

#include <blib/util/Singleton.h>
#include <json/value.h>
#include <glm/glm.hpp>
#include <blib/util/FastDelegate.h>

namespace blib
{
	class Font; 
	class Texture; 
	class SpriteBatch;

	namespace wm
	{
		class Window;
		class Widget;


		class WM : public Singleton<WM>
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
			void setSkin(std::string skinFile);
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


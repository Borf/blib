#pragma once

#include <list>

#include <blib/util/Singleton.h>
#include <json/value.h>
#include <glm/glm.hpp>


namespace blib
{
	namespace gl { class Font; class Texture; class SpriteBatch; };

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

		public:
			WM();
			~WM();

			void addWindow(Window* window);
			void removeWindow(Window* window);
			void setSkin(std::string skinFile);
			void setFont(gl::Font* font);


			void draw(gl::SpriteBatch &spriteBatch);
			bool mousedown(int x, int y);
			bool mouseup(int x, int y, int clickcount);
			bool mousemove(int oldX, int oldY, int x, int y );
			bool mousewheel( int direction, int x, int y );
			bool keyboard(char key);
			bool keyboardSpecial(int key);
			bool hasModalWindow();
			glm::vec4 convertHexColor4( std::string hexColor );
			Json::Value skin;
			gl::Texture* skinTexture;
			gl::Font* font;

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


		};
	}
}


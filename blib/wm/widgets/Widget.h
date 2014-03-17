#pragma once

#include <string>
#include <list>
#include <glm/glm.hpp>

#include <functional>

namespace blib
{
	class SpriteBatch; 
	class Texture;

	namespace wm
	{
		class Widget
		{
		public:
			typedef std::function<void(Widget* widget, int x, int y, int clickCount)> mouseHandler;
			typedef std::function<void(char key)> keyHandler;
			typedef std::function<void(char key)> keySpecialHandler;

			std::list<mouseHandler>		clickHandlers;
			std::list<keyHandler>					keyHandlers;
			std::list<keySpecialHandler>			keySpecialHandlers;
		public:


			int x;
			int y;
			int width;
			int height;
			std::string name;

			bool selected;
			bool hover;


			enum PositionHelp
			{
				NOTHING,
				LEFT,
				RIGHT,
				TOP,
				BOTTOM
			};
			PositionHelp positionHelpLeft;
			PositionHelp positionHelpRight;
			PositionHelp positionHelpTop;
			PositionHelp positionHelpBottom;
	
			Widget();
			virtual ~Widget();
			virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix) = 0;
			virtual Widget* getComponent(std::string name);
			virtual bool inComponent(int x, int y); // x,y are relative to the widget's base, no recursion
			virtual Widget* getComponent(int x, int y); //recursion !

			virtual void mousewheel(int direction, int x, int y) {};
			virtual void mousedown(int x, int y){};
			virtual void mouseup(int x, int y){};
			virtual void mouseclick(int x, int y, int clickcount);
			virtual void mousedrag(int x, int y) {};
			virtual void keyboard(char key);
			virtual void keyboardSpecial(int key);


			void addClickHandler(mouseHandler handler);
			void addKeyHandler(keyHandler handler);
			void addKeySpecialHandler(keySpecialHandler handler);
		};
	}
}

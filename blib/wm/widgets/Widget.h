#pragma once

#include <blib/blib.h>
#include <blib/wm/KeyboardFocusable.h>
#include <blib/wm/MouseClickable.h>

#include <string>
#include <list>
#include <glm/glm.hpp>

#include <functional>

namespace blib
{
	class SpriteBatch; 
	class Texture;
	class Renderer;
	enum class Key;

	namespace wm
	{
		class Widget : public KeyboardFocusable, public MouseClickable
		{
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
			virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const = 0;
			virtual Widget* getComponent(std::string name);
			virtual bool inComponent(int x, int y); // x,y are relative to the widget's base, no recursion
			virtual Widget* getComponent(int x, int y); //recursion !
		};
	}
}

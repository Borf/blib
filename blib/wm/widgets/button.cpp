#include "button.h"

#include <blib/wm/WM.h>
#include <blib/SpriteBatch.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Button::Button( std::string text )
			{
				this->text = text;
				this->width = 100;
				this->height = 25;
			}



			void Button::draw( SpriteBatch &spriteBatch, glm::mat4 matrix)
			{
				if(this->hover)
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["button"], glm::vec2(width, height), glm::vec4(0.7f, 0.7f, 0.9f, 1.0f));
				else
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["button"], glm::vec2(width, height));
				spriteBatch.draw(WM::getInstance()->font, text, glm::translate(matrix, glm::vec3(x+10.0f,y + 5,0)), glm::vec4(0,0,0,1));

			}

		}
	}
}

#include "CloseButton.h"

#include <blib/wm/WM.h>
#include <blib/SpriteBatch.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			CloseButton::CloseButton(std::string text)
			{
				this->width = 25;
				this->height = 25;
			}



			void CloseButton::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				if(this->hover)
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["closebutton"], glm::vec2(width, height), glm::vec4(0.7f, 0.7f, 0.9f, 1.0f));
				else
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["closebutton"], glm::vec2(width, height));
			}

		}
	}
}

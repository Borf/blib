#include "Image.h"

#include <gl/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <blib/wm/wm.h>
#include <blib/Texture.h>
#include <blib/SpriteBatch.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Image::Image(Texture* texture )
			{
				this->texture = texture;
				this->width = 100;
				this->height = 100;
				showBorder = true;
			}



			void Image::draw( SpriteBatch &spriteBatch, glm::mat4 matrix)
			{
				if(showBorder)
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["box"], glm::vec2(width, height));
				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x+2,y+2,0)));
			}


			void Image::setTexture(Texture* texture)
			{
				this->texture = texture;
			}

			Texture* Image::getTexture()
			{
				return this->texture;
			}

		}
	}
}
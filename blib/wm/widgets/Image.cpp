#include "Image.h"

//#include <gl/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <blib/wm/WM.h>
#include <blib/Texture.h>
#include <blib/SpriteBatch.h>
#include <blib/Math.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			Image::Image(Texture* texture)
			{
				this->texInfo = nullptr;
				this->texture = texture;
				this->width = 100;
				this->height = 100;
				showBorder = true;
				scaleAspect = true;
			}

			Image::Image(TextureMap::TexInfo* texInfo)
			{
				this->texInfo = texInfo;
				this->texture = nullptr;
				this->width = 100;
				this->height = 100;
				showBorder = true;
				scaleAspect = true;
			}



			void Image::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				if(showBorder)
					spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["frame"], glm::vec2(width, height), bgcolor);

				if (texture)
				{
//					if (!texture->loaded)
//						return;
					glm::vec2 scale = glm::vec2((width - 2.0f) / texture->originalWidth, (height - 2.0f) / texture->originalHeight);

					if (scaleAspect)
						scale.x = (scale.y = glm::min(scale.x, scale.y));	//uninitialized read in min function?

					spriteBatch.draw(texture, blib::math::easyMatrix(
						glm::vec2(x + 2, y + 2),
						0,
						scale,
						matrix));
				}
				else if (texInfo)
				{
					glm::vec2 scale = glm::vec2((width - 2.0f) / texInfo->width, (height - 2.0f) / texInfo->height);

					if (scaleAspect)
						scale.x = (scale.y = glm::min(scale.x, scale.y));	//uninitialized read in min function?

					spriteBatch.draw(texInfo, blib::math::easyMatrix(
						glm::vec2(x + 2, y + 2),
						0,
						scale,
						matrix));
				}
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

#include "ScrollPanel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <blib/wm/WM.h>
#include <blib/Texture.h>
#include <blib/SpriteBatch.h>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			ScrollPanel::ScrollPanel()
			{
				internalHeight = 1000;
				internalWidth = 1000;
				scrollX = 0;
				scrollY = 0;
				checkVisibility = false;
			}


			void ScrollPanel::draw( SpriteBatch &spriteBatch, glm::mat4 matrix)
			{

				Json::Value skin = WM::getInstance()->skin["list"];
				blib::Texture* texture = WM::getInstance()->skinTexture;

				spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x,y,0)), skin, glm::vec2(width - skin["scroll"]["width"].asInt(), height));
				
				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].asInt(), y, 0)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["buttonup"]["left"].asInt() / (float)texture->originalWidth, skin["scroll"]["buttonup"]["top"].asInt() / (float)texture->originalHeight),
					skin["scroll"]["width"].asInt() / (float)texture->originalWidth, skin["scroll"]["buttonup"]["height"].asInt() / (float)texture->originalHeight));

				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].asInt(), y + height - skin["scroll"]["buttondown"]["height"].asInt(), 0)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["buttondown"]["left"].asInt() / (float)texture->originalWidth, skin["scroll"]["buttondown"]["top"].asInt() / (float)texture->originalHeight),
					skin["scroll"]["width"].asInt() / (float)texture->originalWidth, skin["scroll"]["buttondown"]["height"].asInt() / (float)texture->originalHeight));

				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].asInt(), y + skin["scroll"]["buttonup"]["height"].asInt(), 0)), glm::vec3(1, (height - skin["scroll"]["buttonup"]["height"].asInt() - skin["scroll"]["buttondown"]["height"].asInt()) / (float)skin["scroll"]["background"]["height"].asInt(), 1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["background"]["left"].asInt() / (float)texture->originalWidth, skin["scroll"]["background"]["top"].asInt() / (float)texture->originalHeight),
					skin["scroll"]["width"].asInt() / (float)texture->originalWidth, skin["scroll"]["background"]["height"].asInt() / (float)texture->originalHeight));


			/*	spriteBatch.end();
				spriteBatch.renderState.scissor = true;
				spriteBatch.renderState.scissorArea[0] = (int)matrix[3][0]+x+2;
				spriteBatch.renderState.scissorArea[1] = (int)matrix[3][1] + y + 2;
				spriteBatch.renderState.scissorArea[2] = width - 4 - skin["scroll"]["width"].asInt();
				spriteBatch.renderState.scissorArea[3] = height-4;
				spriteBatch.begin();*/

				matrix = glm::translate(matrix, glm::vec3(x+2,y+2-scrollY,0));
				for (std::list<Widget*>::iterator it = children.begin(); it != children.end(); it++)
				{
					Widget* w = *it;
					if (w->x + w->width < scrollX || w->y + w->height < scrollY)
						continue;
					if (w->x > scrollX + width || w->y > scrollY + height)
						continue;

					w->draw(spriteBatch, matrix);
				}


				/*spriteBatch.end();
				spriteBatch.renderState.scissor = false;
				spriteBatch.begin();*/

			}

			void ScrollPanel::arrangeComponents( int oldWidth, int oldHeight )
			{
			}



			void ScrollPanel::mousewheel( int direction, int x, int y )
			{
				scrollY -= 16 * direction / 120;
				if(scrollY > internalHeight-height)
					scrollY = internalHeight-height;
				if(scrollY < 0)
					scrollY = 0;
			}

			Widget* ScrollPanel::getComponent( int x, int y )
			{
				return Panel::getComponent(x+scrollX+2, y+scrollY+2);
			}

			void ScrollPanel::mousedown( int x, int y )
			{
				Panel::mousedown(x+scrollX+2, y+scrollY+2);
			}

			void ScrollPanel::mouseup( int x, int y )
			{
				Panel::mouseup(x+scrollX+2, y+scrollY+2);
			}

			void ScrollPanel::mouseclick( int x, int y, int clickcount )
			{
				Panel::mouseclick(x+scrollX+2, y+scrollY+2, clickcount);
			}

			void ScrollPanel::mousedrag( int x, int y )
			{
				Panel::mousedrag(x+scrollX+2, y+scrollY+2);
			}
		}
	}
}
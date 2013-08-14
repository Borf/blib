#include "ScrollPanel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <blib/wm/WM.h>
#include <blib/gl/texture.h>
#include <blib/gl/SpriteBatch.h>


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
			}


			void ScrollPanel::draw( gl::SpriteBatch &spriteBatch, glm::mat4 matrix)
			{

				Json::Value skin = WM::getInstance()->skin["list"];

				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), skin, glm::vec2(width - skin["scroll"]["width"].asInt(), height));
				
				
				/*
				glBindTexture(GL_TEXTURE_2D, WM::getInstance()->skinTexture->texid);



				GlHelper::drawStretchyRect(x, y, width - skin["scroll"]["width"].asInt(), height, skin);

				int textureWidth = WM::getInstance()->skinTexture->width;
				int textureHeight = WM::getInstance()->skinTexture->width;

				float tx = 1.0f/textureWidth;
				float ty = 1.0f/textureHeight;

				glBegin(GL_QUADS);
				glVertexAttrib3f(1, 1, 1, 1);	


				//button top
				GlHelper::drawRect(
					(skin["scroll"]["buttonup"]["left"].asInt()) * tx,
					(skin["scroll"]["buttonup"]["top"].asInt()) * ty,
					x+width-skin["scroll"]["width"].asInt(),
					y,
					(skin["scroll"]["buttonup"]["left"].asInt() + skin["scroll"]["width"].asInt()) * tx,
					(skin["scroll"]["buttonup"]["top"].asInt() + skin["scroll"]["buttonup"]["height"].asInt()) * ty,
					x + width,
					y + skin["scroll"]["buttonup"]["height"].asInt()
					);

				//background
				GlHelper::drawRect(
					(skin["scroll"]["background"]["left"].asInt()) * tx,
					(skin["scroll"]["background"]["top"].asInt()) * ty,
					x+width-skin["scroll"]["width"].asInt(),
					y + skin["scroll"]["buttonup"]["height"].asInt(),
					(skin["scroll"]["background"]["left"].asInt() + skin["scroll"]["width"].asInt()) * tx,
					(skin["scroll"]["background"]["top"].asInt() + skin["scroll"]["background"]["height"].asInt()) * ty,
					x + width,
					y + height - skin["scroll"]["buttondown"]["height"].asInt()
					);
				//button bottom
				GlHelper::drawRect(
					(skin["scroll"]["buttondown"]["left"].asInt()) * tx,
					(skin["scroll"]["buttondown"]["top"].asInt()) * ty,
					x+width-skin["scroll"]["width"].asInt(),
					y + height - skin["scroll"]["buttondown"]["height"].asInt(),
					(skin["scroll"]["buttondown"]["left"].asInt() + skin["scroll"]["width"].asInt()) * tx,
					(skin["scroll"]["buttondown"]["top"].asInt() + skin["scroll"]["buttondown"]["height"].asInt()) * ty,
					x + width,
					y + height
					);
				glEnd();


				glScissor((int)shader->matrix[3][0]+x+2,
							shader->height-(int)shader->matrix[3][1]-height+2,
							width-skin["scroll"]["width"].asInt()-4,
							height-4);
				glEnable(GL_SCISSOR_TEST);
				glm::mat4 matrix = shader->matrix;
				shader->setMatrix(glm::translate(shader->matrix, glm::vec3(x+2,y+2-scrollY,0)));
				shader->setMatrix(matrix);
				glDisable(GL_SCISSOR_TEST);
				*/

				ContainerWidget::draw(spriteBatch, glm::translate(matrix, glm::vec3(x+2,y+2-scrollY,0)));

			}

			void ScrollPanel::arrangeComponents( int oldWidth, int oldHeight )
			{
			}



			void ScrollPanel::mousewheel( int direction, int x, int y )
			{
				scrollY -= 16 * direction;
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
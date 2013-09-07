#include "list.h"

#include <blib/wm/wm.h>
#include <blib/Texture.h>
#include <blib/SpriteBatch.h>
#include <blib/math/Rectangle.h>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define NOMINMAX
#include <windows.h>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{

			List::List( )
			{
				this->width = 100;
				this->height = 25;
				scrollPosition = 0;
				selectedItem = -1;
			}



			void List::draw( SpriteBatch &spriteBatch, glm::mat4 matrix)
			{

				spriteBatch.drawStretchyRect(WM::getInstance()->skinTexture, glm::translate(matrix, glm::vec3(x,y,0)), WM::getInstance()->skin["list"], glm::vec2(width, height));
				
				
				
				/*
				glBindTexture(GL_TEXTURE_2D, WM::getInstance()->skinTexture->texid);
	
				Json::Value skin = WM::getInstance()->skin["list"];


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


				glScissor((int)shader->matrix[3][0],shader->height-(int)shader->matrix[3][1]-height,width,height);
				glEnable(GL_SCISSOR_TEST);

				if(selectedItem != -1)
				{
					shader->setColor(WM::getInstance()->convertHexColor4(skin["selectcolor"].asString()));
					GlHelper::drawStretchyRect(x+2, y + 4 + 12*selectedItem-scrollPosition, width - 4 - skin["scroll"]["width"].asInt(), 12, skin);
					shader->setColor(glm::vec4(1,1,1,1));
				}

				for(int i = scrollPosition/12; i < glm::min((int)items.size(), scrollPosition/12+(int)ceil(height/12.0)); i++)
				{
					if(i == selectedItem)
						shader->setColor(WM::getInstance()->convertHexColor4(skin["selectfontcolor"].asString()));
					WM::getInstance()->font->Render(items[i].c_str(), -1, FTPoint(x+2,y-14 - 12*i+scrollPosition ));
					if(i == selectedItem)
						shader->setColor(glm::vec4(1,1,1,1));
				}
				glDisable(GL_SCISSOR_TEST);*/
			}

			void List::mousewheel( int direction, int x, int y )
			{
				scrollPosition -= direction*10;
				if(scrollPosition < 0)
					scrollPosition = 0;
				if(scrollPosition > 12*(int)items.size()-12)
					scrollPosition = 12*items.size()-12;
			}


			void List::mouseclick(int x, int y, int clickcount)
			{
				Widget::mouseclick(x, y, clickcount);
				Json::Value skin = WM::getInstance()->skin["list"];
				if(x-this->x < width - skin["scroll"]["width"].asInt())
				{
					selectedItem = ((y-this->y+scrollPosition)/12);
				}
			}

			void List::keyboardSpecial( int key )
			{
				Widget::keyboardSpecial(key);
				if(key == VK_DOWN)
					selectedItem = glm::min((int)items.size()-1, selectedItem+1);
				if(key == VK_UP)
					selectedItem = glm::max(0, selectedItem-1);

			}

		}
	}
}
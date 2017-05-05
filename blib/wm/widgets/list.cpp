#include <blib/json.hpp>
#include "list.h"

#include <blib/wm/WM.h>
#include <blib/Texture.h>
#include <blib/SpriteBatch.h>
#include <blib/math/Rectangle.h>
#include <blib/KeyListener.h>
#include <blib/Math.h>

//#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef WIN32
#include <windows.h>
#endif

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
				multiselect = false;

				addClickHandler([this](int x, int y, int clickCount)
				{
					json skin = WM::getInstance()->skin["list"];
					if (x - this->x < width - skin["scroll"]["width"].get<int>())
					{
						selectedItems.clear();
						selectedItems.push_back((y - this->y + scrollPosition) / 12);
						return true;
					}
					return false;
				});


				addScrollHandler([this](int x, int y, int delta)
				{
					scrollPosition -= delta / abs(delta)*height / 2;
					if (scrollPosition > 12 * (int)items.size() - 12)
						scrollPosition = 12 * (int)items.size() - 12;
					if (scrollPosition < 0)
						scrollPosition = 0;
					return true;
				});

				addKeyDownHandler([this](blib::Key key)
				{
					if (!selectedItems.empty())
					{
						if (key == blib::Key::DOWN)
						{
							selectedItems[0] = glm::min((int)items.size() - 1, selectedItems[0] + 1);
							return true;
						}
						if (key == blib::Key::UP)
						{
							selectedItems[0] = glm::max(0, selectedItems[0] - 1);
							return true;
						}
					}
					return false;
				});



			}



			void List::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				json skin = WM::getInstance()->skin["list"];
				Texture* texture = WM::getInstance()->skinTexture;


				spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x,y,0)), skin, glm::vec2(width - skin["scroll"]["width"].get<int>(), height));
				
				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y, 0)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["buttonup"]["left"].get<int>() / (float)texture->originalWidth, skin["scroll"]["buttonup"]["top"].get<int>() / (float)texture->originalHeight),
					skin["scroll"]["width"].get<int>() / (float)texture->originalWidth, skin["scroll"]["buttonup"]["height"].get<int>() / (float)texture->originalHeight));

				spriteBatch.draw(texture, glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + height - skin["scroll"]["buttondown"]["height"].get<int>(), 0)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["buttondown"]["left"].get<int>() / (float)texture->originalWidth, skin["scroll"]["buttondown"]["top"].get<int>() / (float)texture->originalHeight),
					skin["scroll"]["width"].get<int>() / (float)texture->originalWidth, skin["scroll"]["buttondown"]["height"].get<int>() / (float)texture->originalHeight));

				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + skin["scroll"]["buttonup"]["height"].get<int>(), 0)), glm::vec3(1,(height-skin["scroll"]["buttonup"]["height"].get<int>() - skin["scroll"]["buttondown"]["height"].get<int>()) / (float)skin["scroll"]["background"]["height"].get<int>(),1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["background"]["left"].get<int>() / (float)texture->originalWidth, skin["scroll"]["background"]["top"].get<int>() / (float)texture->originalHeight),
					skin["scroll"]["width"].get<int>() / (float)texture->originalWidth, skin["scroll"]["background"]["height"].get<int>() / (float)texture->originalHeight));


				/*
				glBindTexture(GL_TEXTURE_2D, WM::getInstance()->skinTexture->texid);
	


				GlHelper::drawStretchyRect(x, y, width - skin["scroll"]["width"].get<int>(), height, skin);

				int textureWidth = WM::getInstance()->skinTexture->width;
				int textureHeight = WM::getInstance()->skinTexture->width;

				float tx = 1.0f/textureWidth;
				float ty = 1.0f/textureHeight;

				glBegin(GL_QUADS);
				glVertexAttrib3f(1, 1, 1, 1);	


				//button top
				GlHelper::drawRect(
					(skin["scroll"]["buttonup"]["left"].get<int>()) * tx,
					(skin["scroll"]["buttonup"]["top"].get<int>()) * ty,
					x+width-skin["scroll"]["width"].get<int>(),
					y,
					(skin["scroll"]["buttonup"]["left"].get<int>() + skin["scroll"]["width"].get<int>()) * tx,
					(skin["scroll"]["buttonup"]["top"].get<int>() + skin["scroll"]["buttonup"]["height"].get<int>()) * ty,
					x + width,
					y + skin["scroll"]["buttonup"]["height"].get<int>()
					);

				//background
				GlHelper::drawRect(
					(skin["scroll"]["background"]["left"].get<int>()) * tx,
					(skin["scroll"]["background"]["top"].get<int>()) * ty,
					x+width-skin["scroll"]["width"].get<int>(),
					y + skin["scroll"]["buttonup"]["height"].get<int>(),
					(skin["scroll"]["background"]["left"].get<int>() + skin["scroll"]["width"].get<int>()) * tx,
					(skin["scroll"]["background"]["top"].get<int>() + skin["scroll"]["background"]["height"].get<int>()) * ty,
					x + width,
					y + height - skin["scroll"]["buttondown"]["height"].get<int>()
					);
				//button bottom
				GlHelper::drawRect(
					(skin["scroll"]["buttondown"]["left"].get<int>()) * tx,
					(skin["scroll"]["buttondown"]["top"].get<int>()) * ty,
					x+width-skin["scroll"]["width"].get<int>(),
					y + height - skin["scroll"]["buttondown"]["height"].get<int>(),
					(skin["scroll"]["buttondown"]["left"].get<int>() + skin["scroll"]["width"].get<int>()) * tx,
					(skin["scroll"]["buttondown"]["top"].get<int>() + skin["scroll"]["buttondown"]["height"].get<int>()) * ty,
					x + width,
					y + height
					);
				glEnd();


				glScissor((int)shader->matrix[3][0],shader->height-(int)shader->matrix[3][1]-height,width,height);
				glEnable(GL_SCISSOR_TEST);

*/


				spriteBatch.end();
				spriteBatch.renderState.scissor = true;
				spriteBatch.renderState.scissorArea[0] = (int)matrix[3][0] + x + 2;
				spriteBatch.renderState.scissorArea[1] = (int)matrix[3][1] + y + 2;
				spriteBatch.renderState.scissorArea[2] = width - 4 - skin["scroll"]["width"].get<int>();
				spriteBatch.renderState.scissorArea[3] = height - 4;
				spriteBatch.begin();

//				if (!multiselect && selectedItem() >= 0 && selectedItem() < (int)items.size())
//					spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x + 2, y + 4 + 12 * selectedItem() - scrollPosition, 0)), skin, glm::vec2(width - 4 - skin["scroll"]["width"].get<int>(), 12), WM::getInstance()->convertHexColor4(skin["selectcolor"].get<std::string>()));


				int selectionIndex = 0;

				for (int i = scrollPosition / 12; i < glm::min((int)items.size(), scrollPosition / 12 + (int)ceil(height / 12.0)); i++)
				{
					if (!selectedItems.empty() && selectionIndex < (int)selectedItems.size())
					{
						if (selectedItems[selectionIndex] == i)
						{
							spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x + 2, y + 4 + 12 * i - scrollPosition, 0)), skin, glm::vec2(width - 4 - skin["scroll"]["width"].get<int>(), 12), WM::getInstance()->convertHexColor4(skin["selectcolor"].get<std::string>()));
							selectionIndex++;
						}

					}


					spriteBatch.draw(WM::getInstance()->font, items[i], 
						blib::math::easyMatrix(glm::vec2(x + 2, y + 12 * i - scrollPosition), matrix), 
						WM::getInstance()->convertHexColor4(i == selectedItem() ? skin["selectfontcolor"].get<std::string>() : skin["fontcolor"].get<std::string>()));
				}

				spriteBatch.end();
				spriteBatch.renderState.scissor = false;
				spriteBatch.begin();


			}

			int List::selectedItem() const
			{
				return selectedItems.empty() ? -1 : selectedItems[0];
			}

			bool List::selectedItem(int item) const
			{
				return false;
			}

		}
	}
}

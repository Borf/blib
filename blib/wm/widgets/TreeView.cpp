#include "TreeView.h"
#include <blib/SpriteBatch.h>
#include <blib/wm/WM.h>
#include <blib/Math.h>
#include <glm/gtc/matrix_transform.hpp>


namespace blib
{
	namespace wm
	{
		namespace widgets
		{

			TreeView::TreeView() : Widget()
			{
				scrollPosition = 0;
				selectedItem = -1;
				root = NULL;
			//	buildList();

				addClickHandler([this](int x, int y, int clickCount)
				{
					Json::Value skin = WM::getInstance()->skin["list"];
					if (x - this->x < width - skin["scroll"]["width"].asInt())
					{
						selectedItem = (y - this->y + scrollPosition) / 12;
						if (clickCount == 2)
							if (currentList[selectedItem].second->hasChildren())
							{
								currentList[selectedItem].second->opened = !currentList[selectedItem].second->opened;
								buildList();
							}
						return true;
					}
					return false;
				});


				addScrollHandler([this](int x, int y, int delta)
				{
					scrollPosition -= delta / abs(delta)*height / 2;
					if (scrollPosition > 12 * (int)currentList.size() - 12)
						scrollPosition = 12 * currentList.size() - 12;
					if (scrollPosition < 0)
						scrollPosition = 0;
					return true;
				});

				addKeyDownHandler([this](blib::Key key)
				{
					if (key == blib::Key::DOWN)
					{
						selectedItem = glm::min((int)currentList.size(), selectedItem + 1);
						return true;
					}
					if (key == blib::Key::UP)
					{
						selectedItem = glm::max((int)currentList.size(), selectedItem - 1);
						return true;
					}
					return false;
				});

			}

			void TreeView::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				Json::Value skin = WM::getInstance()->skin["list"];
				Texture* texture = WM::getInstance()->skinTexture;


				spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x, y, 0)), skin, glm::vec2(width - skin["scroll"]["width"].asInt(), height));

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



				spriteBatch.end();
				spriteBatch.renderState.scissor = true;
				spriteBatch.renderState.scissorArea[0] = (int)matrix[3][0] + x + 2;
				spriteBatch.renderState.scissorArea[1] = (int)matrix[3][1] + y + 2;
				spriteBatch.renderState.scissorArea[2] = width - 4 - skin["scroll"]["width"].asInt();
				spriteBatch.renderState.scissorArea[3] = height - 4;
				spriteBatch.begin();




				for (int i = scrollPosition / 12; i < glm::min((int)currentList.size(), scrollPosition / 12 + (int)ceil(height / 12.0)); i++)
				{
					if (i == selectedItem)
						spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x + 2, y + 4 + 12 * i - scrollPosition, 0)), skin, glm::vec2(width - 4 - skin["scroll"]["width"].asInt(), 12), WM::getInstance()->convertHexColor4(skin["selectcolor"].asString()));


					spriteBatch.draw(WM::getInstance()->font, currentList[i].first,
						blib::math::easyMatrix(glm::vec2(x + 2, y + 12 * i - scrollPosition), matrix),
						WM::getInstance()->convertHexColor4(i == selectedItem ? skin["selectfontcolor"].asString() : skin["fontcolor"].asString()));
				}






				spriteBatch.end();
				spriteBatch.renderState.scissor = false;
				spriteBatch.begin();



			}

			void TreeView::buildList()
			{
				currentList.clear();

				auto str_repeat = [](const int count, char c) {
					char ret[50];
					memset(ret, c, count);
					ret[count] = 0;
					return std::string(ret);
				};

				
				root->foreachLevel([&,this](TreeNode* el, int level){
					if (level == 0)
						return;
					if (!el->isVisible())
						return;
					
					std::string strVal = "";
					strVal += str_repeat((level-1) * 3, ' ');

					if (!el->hasChildren())
						strVal += " ";
					else if (el->opened)
						strVal += "-";
					else
						strVal += "+";

					strVal += el->text;

					currentList.push_back(std::pair<std::string, TreeNode*>(strVal, el));
				});




			}


		}
	}
}
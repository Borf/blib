#include "TreeView.h"
#include <blib/SpriteBatch.h>
#include <blib/wm/WM.h>
#include <blib/Math.h>
#include <blib/Font.h>
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
				buildList();

				addClickHandler([this](int x, int y, int clickCount)
				{
					json skin = WM::getInstance()->skin["list"];
					if (x - this->x < width - skin["scroll"]["width"].get<int>())
					{
						selectedItem = (y - this->y + scrollPosition) / 12;
						if (selectedItem >= (int)currentList.size())
							selectedItem = currentList.size() - 1;
						if (clickCount == 2 || x - this->x < 10*currentList[selectedItem].second->level+10)
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
					if (scrollPosition > (12 * (int)currentList.size() - (height-4)))
						scrollPosition = 12 * currentList.size() - (height-4);
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
				json skin = WM::getInstance()->skin["list"];
				Texture* texture = WM::getInstance()->skinTexture;


				spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x, y, 0)), skin, glm::vec2(width - skin["scroll"]["width"].get<int>(), height));

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

				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + skin["scroll"]["buttonup"]["height"].get<int>(), 0)), glm::vec3(1, (height - skin["scroll"]["buttonup"]["height"].get<int>() - skin["scroll"]["buttondown"]["height"].get<int>()) / (float)skin["scroll"]["background"]["height"].get<int>(), 1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["background"]["left"].get<int>() / (float)texture->originalWidth, skin["scroll"]["background"]["top"].get<int>() / (float)texture->originalHeight),
					skin["scroll"]["width"].get<int>() / (float)texture->originalWidth, skin["scroll"]["background"]["height"].get<int>() / (float)texture->originalHeight));



				float pages = (currentList.size() * 12) / (float)height;
				if (pages < 1)
					pages = 1;
				float scrollHeight = (height - (skin["scroll"]["buttonup"]["height"].get<int>() + skin["scroll"]["buttondown"]["height"].get<int>())) / (float)pages;
				float scrollFac = scrollPosition / (float)((currentList.size() * 12) - height);
				float scrollHandlePosition = scrollFac * (height - (skin["scroll"]["buttonup"]["height"].get<int>() + skin["scroll"]["buttondown"]["height"].get<int>() + scrollHeight));

				float margin = (skin["scroll"]["handle"]["height"].get<float>() - skin["scroll"]["handle"]["center"].get<float>()) / 2.0f;

				//handle center
				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + skin["scroll"]["buttonup"]["height"].get<int>() + scrollHandlePosition + margin, 0)), glm::vec3(1, (scrollHeight - 2 * margin) / (skin["scroll"]["handle"]["height"].get<float>() - 2 * margin), 1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["handle"]["left"].get<int>() / (float)texture->originalWidth, (skin["scroll"]["handle"]["top"].get<int>() + margin) / (float)texture->originalHeight),
					skin["scroll"]["handle"]["width"].get<int>() / (float)texture->originalWidth, (skin["scroll"]["handle"]["height"].get<int>() - 2 * margin) / (float)texture->originalHeight));


				//handle top
				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + skin["scroll"]["buttonup"]["height"].get<int>() + scrollHandlePosition, 0)), glm::vec3(1, 1, 1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["handle"]["left"].get<int>() / (float)texture->originalWidth, skin["scroll"]["handle"]["top"].get<int>() / (float)texture->originalHeight),
					skin["scroll"]["handle"]["width"].get<float>() / (float)texture->originalWidth, margin / (float)texture->originalHeight));
				//handle bottom
				spriteBatch.draw(texture, glm::scale(glm::translate(matrix, glm::vec3(x + width - skin["scroll"]["width"].get<int>(), y + skin["scroll"]["buttonup"]["height"].get<int>() + scrollHandlePosition + scrollHeight - margin, 0)), glm::vec3(1, 1, 1)),
					glm::vec2(0, 0),
					blib::math::Rectangle(
					glm::vec2(skin["scroll"]["handle"]["left"].get<int>() / (float)texture->originalWidth, (skin["scroll"]["handle"]["top"].get<int>() + skin["scroll"]["handle"]["height"].get<int>() - margin) / (float)texture->originalHeight),
					skin["scroll"]["handle"]["width"].get<float>() / (float)texture->originalWidth, margin / (float)texture->originalHeight));



				spriteBatch.end();
				spriteBatch.renderState.scissor = true;
				spriteBatch.renderState.scissorArea[0] = (int)matrix[3][0] + x + 2;
				spriteBatch.renderState.scissorArea[1] = (int)matrix[3][1] + y + 2;
				spriteBatch.renderState.scissorArea[2] = width - 4 - skin["scroll"]["width"].get<int>();
				spriteBatch.renderState.scissorArea[3] = height - 4;
				spriteBatch.begin();



				blib::Font* font = WM::getInstance()->font;
				for (int i = scrollPosition / 12; i < glm::min((int)currentList.size(), scrollPosition / 12 + (int)ceil(height / 12.0)); i++)
				{
					if (i == selectedItem)
						spriteBatch.drawStretchyRect(texture, glm::translate(matrix, glm::vec3(x + 10 + 10*currentList[i].second->level, y + 2 + 12 * i - scrollPosition, 0)), skin, glm::vec2(glm::min(width - 4.0f - skin["scroll"]["width"].get<int>(), font->textlen(currentList[i].second->text)), 13), WM::getInstance()->convertHexColor4(skin["selectcolor"].get<std::string>()));

					//draw + or - 
					if (currentList[i].second->hasChildren())
					{
						if (currentList[i].second->opened)
							spriteBatch.draw(texture, blib::math::easyMatrix(glm::vec2(x - 2 + 10 * currentList[i].second->level, y + 2 + 12 * i - scrollPosition), matrix), glm::vec2(0, 0), blib::math::Rectangle(0.0f, 100.0f / texture->originalHeight, 9.0f / texture->originalWidth, 9.0f / texture->originalHeight));
						else
							spriteBatch.draw(texture, blib::math::easyMatrix(glm::vec2(x - 2 + 10 * currentList[i].second->level, y + 2 + 12 * i - scrollPosition), matrix), glm::vec2(0, 0), blib::math::Rectangle(10.0f / texture->originalWidth, 100.0f / texture->originalHeight, 9.0f / texture->originalWidth, 9.0f / texture->originalHeight));
					}


					spriteBatch.draw(font, currentList[i].first,
						blib::math::easyMatrix(glm::vec2(x + 2, 12+y + 12 * i - scrollPosition), matrix),
						WM::getInstance()->convertHexColor4(i == selectedItem ? skin["selectfontcolor"].get<std::string>() : skin["fontcolor"].get<std::string>()));
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

				if (root)
					root->foreachLevel([&,this](TreeNode* el, int level){
						el->level = level;
						if (level == 0)
							return;
						if (!el->isVisible())
							return;
					
						std::string strVal = "";
						strVal += str_repeat((level-1) * 3, ' ');

/*						if (!el->hasChildren())
							strVal += "\t";
						else if (el->opened)
							strVal += "-\t";
						else
							strVal += "+\t";*/

						strVal += "    ";

						strVal += el->text;

						currentList.push_back(std::pair<std::string, TreeNode*>(strVal, el));
					});




			}


		}
	}
}
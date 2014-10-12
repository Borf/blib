#include "BmlBox.h"

#include <blib/ResourceManager.h>
#include <blib/SpriteBatch.h>
#include <blib/wm/WM.h>
#include <blib/util/Tree.h>

#include <glm/gtc/matrix_transform.hpp>

namespace blib
{
	namespace wm
	{
		namespace widgets
		{
			namespace bml
			{
				class BmlRenderBlock
				{
				public:
					glm::ivec2 topleft;
					glm::ivec2 size;

					glm::ivec2 cursor;


					BmlRenderBlock(const glm::ivec2 &topleft, const glm::ivec2 &size)
					{
						this->topleft = topleft;
						this->size = size;
						cursor = glm::ivec2(0, 0);
					}
				};



				class Element
				{
				public:
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const = 0;
				};

				template<class T>
				class ElementContainer : public Element, public blib::util::DynTree<T>
				{
				public:
					void add(T* t)
					{
						children.push_back(t);
					}
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						BmlRenderBlock subBox = box;
						for (std::list<T*>::const_iterator it = children.cbegin(); it != children.cend(); it++)
						{
							BmlRenderBlock b2 = (*it)->draw(spriteBatch, matrix, renderData, fontProperties, subBox);
							subBox.topleft.y += b2.size.y;
						}
						return box;
					}
				};


				class TextNode : public Element
				{
				public:
					std::string text;
					TextNode(const std::string &text)
					{
						this->text = text;
					}
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						spriteBatch->draw(renderData->getFont(fontProperties), text, glm::translate(matrix, glm::vec3(box.topleft,0)), fontProperties.color);
						box.cursor.y += 14;
						return box;
					}
				};

				class Header : public ElementContainer<Element>
				{
				public:
					Header() {};
					Header(Element* el) { add(el); };
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						fontProperties.size += 0.5f;
						return ElementContainer<Element>::draw(spriteBatch, matrix, renderData, fontProperties, box);
					}
				};

				class Bold : public ElementContainer<Element>
				{
				public:
					Bold(Element* el) { add(el); };
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						fontProperties.bold = true;
						return ElementContainer<Element>::draw(spriteBatch, matrix, renderData, fontProperties, box);
					}
				};

				class ListItem : public ElementContainer<Element>
				{
				public:
					ListItem(Element* el) { add(el); };
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const;
				};

				class List : public ElementContainer<ListItem>
				{
				public:
					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const;
				};

				class Paragraph : public ElementContainer<Element>
				{
				public:
					Paragraph(){ };
					Paragraph(Element* el) { add(el); };

					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						box.topleft += glm::ivec2(5, 5);
						box.size -= glm::ivec2(10, 10);
						return ElementContainer::draw(spriteBatch, matrix, renderData, fontProperties, box);
					}

				};



				class Document : public ElementContainer<Element>
				{
				public:
/*					virtual BmlRenderBlock draw(SpriteBatch* spriteBatch, glm::mat4 matrix, const BmlRenderData* renderData, BmlFontProperties fontProperties, BmlRenderBlock box) const
					{
						ElementContainer
					}*/
				};








			}





			BmlBox::BmlBox(ResourceManager* resourceManager)
			{
				renderData.resourceManager = resourceManager;
				bml::BmlFontProperties font;



			}

			


			void BmlBox::draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const
			{
				document->draw(&spriteBatch, glm::translate(matrix, glm::vec3(x, y, 0)), &renderData, bml::BmlFontProperties(), bml::BmlRenderBlock(glm::ivec2(0, 0), glm::ivec2(width, 0)));


			//	spriteBatch.draw(WM::getInstance()->font, text, glm::translate(matrix, glm::vec3(x, y, 0)), glm::vec4(0, 0, 0, 1));
			}

			void BmlBox::setBml(std::string data)
			{
				this->text = data;

			}


		}
	}
}









/*



= Header =
This is some text. Then there's some ways to add layouts
*bold text*
_italic text_
[pagebookmark|You can add links]
group text together in paragraphs using double newlines

this will be a new paragraph with text. blablabla bla
- This is a listitem
- this is another listitem
- and another one

== header2 ==
this is a little header
yay


> This will be in a panel
> inside the panel you can draw more things
> yay more text
> - listitem
> - another listitem
> - etc


*/
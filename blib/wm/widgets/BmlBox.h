#pragma once

#include "Widget.h"
#include <map>

#include <glm/gtc/matrix_transform.hpp>

#include <blib/ResourceManager.h>
#include <blib/SpriteBatch.h>

namespace blib
{
	class ResourceManager;
	class Font;

	namespace wm
	{
		namespace widgets
		{
			namespace bml
			{
				class Document;

				class BmlFontProperties
				{
				public:
					float size;
					bool bold, italic;
					std::string font;
					glm::vec4 color;
					BmlFontProperties()
					{
						size = 1;
						bold = italic = false;
						font = "consolas";
						color = glm::vec4(0, 0, 0, 1);
					}
				};

				class BmlRenderData
				{
				public:
					ResourceManager* resourceManager;
					class FontData
					{
					public:
						blib::Font* normal;
						blib::Font* bold;
						blib::Font* italic;
					};
					std::map<std::string, FontData> fonts;

					blib::Font* getFont(const BmlFontProperties& properties) const
					{
						return const_cast<BmlRenderData*>(this)->getFont(properties); //ew
					}

					blib::Font* getFont(const BmlFontProperties& properties)
					{
						if (fonts.find(properties.font) == fonts.end())
						{
							fonts[properties.font] = FontData();
							fonts[properties.font].normal = resourceManager->getResource<blib::Font>(properties.font);
							fonts[properties.font].bold = resourceManager->getResource<blib::Font>(properties.font + ".bold");
							fonts[properties.font].italic = resourceManager->getResource<blib::Font>(properties.font + ".italic");
						}

						if (properties.bold && properties.italic)
							return NULL;
						if (properties.bold)
							return fonts[properties.font].bold;
						else if (properties.italic)
							return fonts[properties.font].italic;

						return fonts[properties.font].normal;
					}

				};


				class Command
				{
				public:
					virtual void draw(SpriteBatch &spriteBatch, const glm::mat4 &matrix, glm::vec2 &cursor, const BmlRenderData &renderData) const = 0;
				};

				class Text : public Command
				{
				public:
					std::string text;
					BmlFontProperties font;
					glm::ivec2 margins;

					Text(const std::string &text, const BmlFontProperties &font, const glm::ivec2 margins)
					{
						this->text = text;
						this->font = font;
						this->margins = margins;
					}


					void draw(SpriteBatch &spriteBatch, const glm::mat4 &matrix, glm::vec2 &cursor, const BmlRenderData &renderData) const
					{
						glm::vec2 newPos = spriteBatch.draw(renderData.getFont(font), text, glm::translate(matrix, glm::vec3(margins.x, 0, 0)), font.color, cursor, margins.y - margins.x);
						cursor.x = newPos.x;
						cursor.y = newPos.y;
					}

				};

				class NewLine : public Command
				{
				public:
					int lineHeight;

					NewLine(int lineHeight = 12)
					{
						this->lineHeight = lineHeight;
					}
					void draw(SpriteBatch &spriteBatch, const glm::mat4 &matrix, glm::vec2 &cursor, const BmlRenderData &renderData) const
					{
						cursor.y += lineHeight;
						cursor.x = 0;
					}
				};



			}



			class BmlBox : public Widget
			{
				std::string text;


				bml::BmlRenderData renderData;
				std::list<bml::Command*> commands;

			public:
				BmlBox(ResourceManager* resourceManager);
				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;



				void setBml(std::string data);

			};
		}
	}
}
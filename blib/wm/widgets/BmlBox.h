#pragma once

#include "Widget.h"
#include <map>

#include <blib/ResourceManager.h>

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
						}

						if (properties.bold && properties.italic)
							return NULL;
						if (properties.bold)
							return NULL;
						else if (properties.italic)
							return NULL;

						return fonts[properties.font].normal;
					}

				};


				class Command
				{
				public:
					virtual void draw(SpriteBatch* spriteBatch, glm::ivec2 &cursor) const = 0;
				};

				class Text : Command
				{
				public:
					std::string text;
					BmlFontProperties font;
					glm::ivec2 margins;

					void draw(SpriteBatch* spriteBatch, glm::ivec2 &cursor)
					{

					}

				};

				class NewLine : Command
				{
				public:
					float lineHeight;
					void draw(SpriteBatch* spriteBatch, glm::ivec2 &cursor)
					{
						cursor.y += lineHeight;
						cursor.x = 0;
					}
				};

				std::list<Command> commands;


			}



			class BmlBox : public Widget
			{
				std::string text;

				bml::BmlRenderData renderData;
				bml::Document* document;

			public:
				BmlBox(ResourceManager* resourceManager);
				virtual void draw(SpriteBatch &spriteBatch, glm::mat4 matrix, Renderer* renderer) const;



				void setBml(std::string data);

			};
		}
	}
}
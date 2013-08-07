#pragma once

#include <string>
#include <map>

namespace blib
{
	namespace gl
	{
		class Texture;

		class Glyph
		{
		public:
			int id,x,y,width,height,xoffset,yoffset,xadvance;
		};


		class Font
		{
		public:
			static std::map<std::string, Font*> fonts;
			static Font* getFontInstance(std::string name);
			static void clearCache();

			void render(std::string text, float scale);
			float textlen(std::string text);
		private:
			Font(std::string file);
			~Font();
			Texture* texture;

			std::map<char, Glyph*> charmap;
		};
	}
}

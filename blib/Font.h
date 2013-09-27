#pragma once

#include <string>
#include <map>

namespace blib
{
	class Texture;
	class ResourceManager;
	namespace gl { class ResourceManager; };
	class Glyph
	{
	public:
		int id,x,y,width,height,xoffset,yoffset,xadvance;
	};


	class Font
	{
		static std::map<std::string, Font*> fonts;
		static Font* getFontInstance(std::string name, ResourceManager* resourceManager);
		static void clearCache();
	//	void render(std::string text, float scale);
		std::map<char, Glyph*> charmap;
		Texture* texture;
		Font(std::string file, ResourceManager* resourceManager);
		~Font();


		friend class gl::ResourceManager;
		friend class SpriteBatch;

	public:
		float textlen(std::string text);

	};
}

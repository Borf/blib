#pragma once

#include <string>
#include <vector>
#include <map>
#include <blib/Resource.h>
#include <blib/util/stb_truetype.h>

namespace blib
{
	class Texture;
	class ResourceManager;
	namespace gl {
		class ResourceManager; namespace direct { class ResourceManager; }	};
	class Glyph
	{
	public:
		int id,x,y,width,height,xoffset,yoffset,xadvance;
	};


	class Font : public Resource
	{
	//	static std::map<std::string, Font*> fonts;
	//	static Font* getFontInstance(std::string name, ResourceManager* resourceManager);
	//	static void clearCache();
	//	void render(std::string text, float scale);
		static std::vector<int> characters;


		std::map<int, Glyph*> charmap;
		Texture* texture;
		Font(const std::string &file, ResourceManager* resourceManager, float size);
		~Font();

		void loadFnt(const std::string &fileName, ResourceManager* resourceManager);
		void loadTtf(const std::string &fileName, ResourceManager* resourceManager, float size);
		stbtt_fontinfo* font;
		stbtt_packedchar fontData[1024];

		const Glyph* getGlyph( const int &character ) const;


		friend class gl::ResourceManager;
		friend class gl::direct::ResourceManager;
		friend class SpriteBatch;
	public:
		float textlen(const std::string &text) const;
		bool utf8 = true;
		static void registerCharacters(const std::string& utf8);
		float lineHeight;

	};
}

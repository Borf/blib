#pragma once

#include <string>
#include <map>
#include <blib/Resource.h>

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
		std::map<int, Glyph*> charmap;
		Texture* texture;
		Font(std::string file, ResourceManager* resourceManager);
		~Font();

		const Glyph* getGlyph( const int &character ) const;

		float lineHeight;

		friend class gl::ResourceManager;
		friend class gl::direct::ResourceManager;
		friend class SpriteBatch;
	public:
		float textlen(const std::string &text) const;
		bool utf8 = true;

	};
}

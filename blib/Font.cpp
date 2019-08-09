#include <blib/Font.h>
#include <blib/Texture.h>
#include <blib/Renderer.h>
#include <blib/util/FileSystem.h>
#include <blib/util/StreamReader.h>
#include <blib/util/Log.h>
#include <blib/ResourceManager.h>
#include <blib/util/stb_rect_pack.h>
#include <blib/util/stb_truetype.h>

using blib::util::Log;

#include <vector>

#include <blib/config.h>
#include <locale>
#include <codecvt>

#if defined(BLIB_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(BLIB_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#endif

#ifndef BLIB_IOS
//#if (!_DLL) && (_MSC_VER >= 1900 /* VS 2015*/) && (_MSC_VER <= 1911 /* VS 2017 */)
#if (!_DLL) && (_MSC_VER >= 1900 /* VS 2015*/)
std::locale::id std::codecvt<char32_t, char, _Mbstatet>::id;
#endif

#endif


namespace blib
{
	/*std::map<std::string, Font*> Font::fonts;

	Font* Font::getFontInstance(std::string name, ResourceManager* resourceManager)
	{
		std::map<std::string, Font*>::iterator it = fonts.find(name);
		if(it == fonts.end())
		{
			Font* font = new Font("assets/fonts/"+name+".fnt", resourceManager);
			fonts[name] = font;
			return font;
		}
		return it->second;
	}

	void Font::clearCache()
	{
		for(std::map<std::string, Font*>::iterator it = fonts.begin(); it != fonts.end(); it++)
			delete it->second;
		fonts.clear();
	}*/


	std::vector<std::string> split(std::string value, std::string seperator)
	{
		std::vector<std::string> ret;
		while(value.find(seperator) != std::string::npos)
		{
			size_t index = value.find(seperator);
			if(index != 0)
				ret.push_back(value.substr(0, index));
			value = value.substr(index+seperator.length());
		}
		ret.push_back(value);
		return ret;
	}



	Font::Font(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer, float size) : Resource("Font: " + fileName)
	{
		if (blib::util::FileSystem::exists(fileName + ".fnt"))
			loadFnt(fileName + ".fnt", resourceManager);
		if (blib::util::FileSystem::exists(fileName + ".ttf"))
			loadTtf(fileName + ".ttf", resourceManager, renderer, size);
	}

	void Font::loadFnt(const std::string &fileName, ResourceManager* resourceManager)
	{
		blib::util::StreamReader* file = new blib::util::StreamReader(blib::util::FileSystem::openRead(fileName));
		if (!file)
			return;
		std::string textureFileName;

		while (!file->eof())
		{
			std::string line = file->getLine();
			if (line.length() < 5)
				continue;

			if (line.substr(0, 5) == "page ")
			{
				textureFileName = line.substr(16);
				textureFileName = textureFileName.substr(0, textureFileName.length() - 1);
			}
			if (line.substr(0, 6) == "common")
			{
				std::vector<std::string> params = split(line.substr(7), " ");
				lineHeight = atof(params[0].substr(11).c_str());
			}
			if (line.substr(0, 5) == "char ")
			{
				std::vector<std::string> params = split(line.substr(5), " ");
				Glyph* glyph = new Glyph();
				glyph->id = 0;

				for (size_t i = 0; i < params.size(); i++)
				{
					std::string name = params[i].substr(0, params[i].find("="));
					if (name == "id")			glyph->id = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "x")			glyph->x = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "y")			glyph->y = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "width")		glyph->width = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "height")		glyph->height = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "xoffset")		glyph->xoffset = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "yoffset")		glyph->yoffset = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "xadvance")		glyph->xadvance = atoi(params[i].substr(name.length() + 1).c_str());
					else if (name == "page" || name == "chnl") {}
					else
						Log::out << "Didn't parse " << name << Log::newline;
				}

				if (glyph->id != 0)
					charmap[glyph->id] = glyph;
				else
					delete glyph;
			}
		}

		Log::out << "Loaded font " << fileName << ", " << charmap.size() << " glyphs" << Log::newline;

		texture = resourceManager->getResource<Texture>("assets/fonts/" + textureFileName);
		delete file;
	}

	void Font::loadTtf(const std::string &fileName, ResourceManager* resourceManager, Renderer* renderer, float size)
	{
		unsigned char* data;
		int dataLen = blib::util::FileSystem::getData(fileName, (char*&)data);
		
		font = new stbtt_fontinfo();

		stbtt_InitFont(font, data, stbtt_GetFontOffsetForIndex(data, 0));

		int oversample = 1;
		
		std::vector<int> characters;
		for (int i = 32; i < 256; i++)
			characters.push_back(i);

		characters.push_back(26085);
		characters.push_back(26412);
		characters.push_back(35486);
		unsigned char* tmpImage = new unsigned char[1024 * 1024];

		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmpImage, 1024, 1024, 0, 2, NULL);
		//if (oversample > 0)
		//	stbtt_PackSetOversampling(&pc, oversample, oversample);
		for (std::size_t i = 0; i < characters.size(); i++)
		{
			stbtt_PackFontRange(&pc, data, 0, size, characters[i], 1, fontData + i);
		}
		stbtt_PackEnd(&pc);

		for (std::size_t i = 0; i < characters.size(); i++)
		{
			Glyph* g = new Glyph();
			
			float x = 0, y = 0;
			stbtt_aligned_quad q;
			stbtt_GetPackedQuad(fontData+i, 1024, 1024, 0, &x, &y, &q, 0);

			int xAdvance = 0, leftSideBearing = 0;
			stbtt_GetGlyphHMetrics(font, characters[i], &xAdvance, &leftSideBearing);
			float scale = stbtt_ScaleForPixelHeight(font, 32);
			

			g->id = characters[i];
			g->xoffset = q.x0*oversample;
			g->yoffset = q.y0*oversample +size;
			g->x = q.s0 * 1024;
			g->y = q.t0 * 1024;
			g->width = (q.s1 - q.s0) * 1024;
			g->height = (q.t1 - q.t0) * 1024;
			g->xadvance = x * oversample;
			charmap[characters[i]] = g;
		}

		unsigned char* imgData = new unsigned char[1024 * 1024 * 4];
		for (int x = 0; x < 1024; x++)
		{
			for (int y = 0; y < 1024; y++)
			{
				imgData[(x + 1024 * y) * 4 + 0] = 255;
				imgData[(x + 1024 * y) * 4 + 1] = 255;
				imgData[(x + 1024 * y) * 4 + 2] = 255;
				imgData[(x + 1024 * y) * 4 + 3] = tmpImage[x + 1024 * y];
			}
		}
		texture = resourceManager->getResource<blib::Texture>(1024, 1024);
		renderer->setTextureSubImage(texture, 0, 0, 1024, 1024, (char*)imgData);
	}


	Font::~Font()
	{
		blib::ResourceManager::getInstance().dispose(texture);
		for(std::map<int, Glyph*>::iterator it = charmap.begin(); it != charmap.end(); it++)
			delete it->second;
		charmap.clear();
	}


	
	float Font::textlen(const std::string &utf8) const
	{
#if defined(_DEBUG) && defined(BLIB_WIN)
		std::wstring text;
		std::wstring space;
		typedef wchar_t ch;
		typedef std::wstring str;

		if (this->utf8)
		{
			text = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(utf8);
			space = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(" ");
		}
		else
		{
			text = std::wstring(utf8.begin(), utf8.end());
			for (int i = 0; i < utf8.size(); i++)
				if (utf8[i] < 0)
					text[i] = (unsigned char)text[i];
			space = std::wstring(1, ' ');
		}

#else
		std::u32string text;
		std::u32string space;
		typedef char32_t ch;
		typedef std::u32string str;

		if (this->utf8)
		{
			text = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(utf8);
			space = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(" ");
		}
		else
		{
			text = std::u32string(utf8.begin(), utf8.end());
			for (int i = 0; i < utf8.size(); i++)
				if (utf8[i] < 0)
					text[i] = (unsigned char)text[i];
			space = std::u32string(1, ' ');
		}
#endif

        float scale = 1;//0.00075f;

		float posX = 0;

		for(size_t i = 0; i < text.size(); i++)
		{
			if(charmap.find(text[i]) == charmap.end())
				continue;
			posX += getGlyph(text[i])->xadvance * scale;

		}
		return posX;
	}

	const Glyph* Font::getGlyph( const int &character ) const
	{
		return charmap.find(character)->second;
	}

}

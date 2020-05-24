#include <blib/Font.h>
#include <blib/Texture.h>
#include <blib/util/FileSystem.h>
#include <blib/util/StreamReader.h>
#include <blib/util/Log.h>
#include <blib/ResourceManager.h>
#include <blib/util/stb_rect_pack.h>
#include <blib/util/stb_truetype.h>
#include <blib/Renderer.h>
#include <blib/gl/Vertex.h>
#include <blib/math/Rectangle.h>
#include <glm/gtc/matrix_transform.hpp>
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

	std::vector<int> Font::characters;

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



	Font::Font(const std::string &fileName, ResourceManager* resourceManager, float size) : Resource("Font: " + fileName)
	{
		if (blib::util::FileSystem::exists(fileName + ".ttf"))
			loadTtf(fileName + ".ttf", resourceManager, size);
		else if (blib::util::FileSystem::exists(fileName + ".fnt"))
			loadFnt(fileName + ".fnt", resourceManager);
		else
			Log::out << "Error loading font: " << fileName << Log::newline;
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

	void Font::registerCharacters(const std::string& utf8)
	{
#if defined(_DEBUG) && defined(BLIB_WIN)
		std::wstring text;
		std::wstring space;
		typedef wchar_t ch;
		typedef std::wstring str;

//		if (this->utf8)
		{
			text = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(utf8);
			space = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes(" ");
		}/*
/*		else
		{
			text = std::wstring(utf8.begin(), utf8.end());
			for (int i = 0; i < utf8.size(); i++)
				if (utf8[i] < 0)
					text[i] = (unsigned char)text[i];
			space = std::wstring(1, ' ');
		}*/

#else
		std::u32string text;
		std::u32string space;
		typedef char32_t ch;
		typedef std::u32string str;

//		if (this->utf8)
		{
			text = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(utf8);
			space = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(" ");
		}
/*		else
		{
			text = std::u32string(utf8.begin(), utf8.end());
			for (int i = 0; i < utf8.size(); i++)
				if (utf8[i] < 0)
					text[i] = (unsigned char)text[i];
			space = std::u32string(1, ' ');
		}*/
#endif

		for (int i = 0; i < text.size(); i++)
		{
			if (std::find(characters.begin(), characters.end(), text[i]) == characters.end())
				characters.push_back(text[i]);
		}
	}

	const int ttfSizeX = 2048;
	const int ttfSizeY = 2048;
	void Font::loadTtf(const std::string &fileName, ResourceManager* resourceManager, float size)
	{
		lineHeight = size;
		unsigned char* data;
		int dataLen = blib::util::FileSystem::getData(fileName, (char*&)data);
		
		font = new stbtt_fontinfo();

		stbtt_InitFont(font, data, stbtt_GetFontOffsetForIndex(data, 0));

		int oversample = 1;
		
		if(Font::characters.size() == 0)
			for (int i = 32; i < 256; i++)
				characters.push_back(i);

		//characters.push_back(26085);
		//characters.push_back(26412);
		//characters.push_back(35486);
		unsigned char* tmpImage = new unsigned char[ttfSizeX * ttfSizeY];

		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmpImage, ttfSizeX, ttfSizeY, 0, 2, NULL);
		//if (oversample > 0)
		//	stbtt_PackSetOversampling(&pc, oversample, oversample);
		for (std::size_t i = 0; i < characters.size(); i++)
		{
			int res = stbtt_PackFontRange(&pc, data, 0, size, characters[i], 1, fontData + i);
			if (res == 0)
				Log::out << "Could not find character " << characters[i] << Log::newline;
		}
		stbtt_PackEnd(&pc);

		for (std::size_t i = 0; i < characters.size(); i++)
		{
			Glyph* g = new Glyph();
			
			float x = 0, y = 0;
			stbtt_aligned_quad q;
			stbtt_GetPackedQuad(fontData+i, ttfSizeX, ttfSizeY, 0, &x, &y, &q, 0);

			int xAdvance = 0, leftSideBearing = 0;
			stbtt_GetGlyphHMetrics(font, characters[i], &xAdvance, &leftSideBearing);
			float scale = stbtt_ScaleForPixelHeight(font, 32);
			

			g->id = characters[i];
			g->xoffset = (int)(q.x0*oversample);
			g->yoffset = (int)(q.y0*oversample +size);
			g->x = (int)(q.s0 * ttfSizeX);
			g->y = (int)(q.t0 * ttfSizeY);
			g->width = (q.s1 - q.s0) * ttfSizeX;
			g->height = (q.t1 - q.t0) * ttfSizeY;
			g->xadvance = x * oversample;
			charmap[characters[i]] = g;
		}

		unsigned char* imgData = new unsigned char[ttfSizeX * ttfSizeY * 4];
		for (int x = 0; x < ttfSizeX; x++)
		{
			for (int y = 0; y < ttfSizeY; y++)
			{
				imgData[(x + ttfSizeY * y) * 4 + 0] = 255;
				imgData[(x + ttfSizeY * y) * 4 + 1] = 255;
				imgData[(x + ttfSizeY * y) * 4 + 2] = 255;
				imgData[(x + ttfSizeY * y) * 4 + 3] = tmpImage[x + ttfSizeY * y];
			}
		}
		texture = resourceManager->getResource<blib::Texture>(ttfSizeX, ttfSizeY, imgData);
	}


	Font::~Font()
	{
		blib::ResourceManager::getInstance().dispose(texture);
		for(std::map<int, Glyph*>::iterator it = charmap.begin(); it != charmap.end(); it++)
			delete it->second;
		charmap.clear();
	}


	void Font::render(blib::Renderer* renderer, blib::RenderState& renderState, const std::string& utf8, const glm::vec4 &color) const
	{
		std::vector<VertexP2T2C4> verts;


		glm::vec2 texFactor(1.0f / texture->width, 1.0f / texture->height);

		float scaleFactor = 1.0f;

		float x = 0;
		float y = 0;
		int lineHeight = 12;


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

		for (size_t i = 0; i < text.size(); i++)
		{
			ch c = text[i];
			if (c == ' ')
			{
				{
					str word = text.substr(i + 1);
					if (word.find(space) != std::string::npos)
						word = word.substr(0, word.find(space));
					int wordLength = 0;
					for (size_t ii = 0; ii < word.length(); ii++)
						if (charmap.find(word[ii]) != charmap.end())
							wordLength += getGlyph(word[ii])->xadvance;
				}
			}

			if (c == '\n')
			{
				x = 0;
				y += lineHeight;
				lineHeight = 12;
				continue;
			}
			if (c == '\t')
			{
				x = ceil((x + 4) / 4) * 4;
				continue;
			}

			if (charmap.find(c) == charmap.end())
			{
				if (c != 0)
					Log::out << "Could not find character " << c << Log::newline;
				continue;
			}
			const Glyph* g = getGlyph(c);
			lineHeight = glm::max(lineHeight, (int)lineHeight);

			glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, 1)), glm::vec3(x + g->xoffset, y + g->yoffset - lineHeight * 0.5, 0));
			blib::math::Rectangle src(g->x * texFactor.x, g->y * texFactor.y, g->width * texFactor.x, g->height * texFactor.y);

			float fw = (float)src.width();
			float fh = (float)src.height();

			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * 0, fh * 0, 0, 1)), glm::vec2(src.topleft.x, src.topleft.y), color)); // 1
			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * 0, fh * texture->originalHeight, 0, 1)), glm::vec2(src.topleft.x, src.bottomright.y), color)); // 2
			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * texture->originalWidth, fh * 0, 0, 1)), glm::vec2(src.bottomright.x, src.topleft.y), color)); // 3

			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * 0, fh * texture->originalHeight, 0, 1)), glm::vec2(src.topleft.x, src.bottomright.y), color)); // 2
			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * texture->originalWidth, fh * 0, 0, 1)), glm::vec2(src.bottomright.x, src.topleft.y), color)); // 3
			verts.push_back(blib::VertexP2T2C4(glm::vec2(transform * glm::vec4(fw * texture->originalWidth, fh * texture->originalHeight, 0, 1)), glm::vec2(src.bottomright.x, src.bottomright.y), color)); //4



			x += g->xadvance;
		}


		renderState.activeTexture[0] = texture;
		renderer->drawTriangles(verts, renderState);
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

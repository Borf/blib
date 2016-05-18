#include <blib/Font.h>
#include <blib/Texture.h>
#include <blib/util/FileSystem.h>
#include <blib/util/StreamReader.h>
#include <blib/util/Log.h>
#include <blib/ResourceManager.h>

using blib::util::Log;

#include <vector>

#include <blib/config.h>

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



	Font::Font(std::string fileName, ResourceManager* resourceManager) : Resource("Font: " + fileName)
	{
		blib::util::StreamReader* file = new blib::util::StreamReader(blib::util::FileSystem::openRead(fileName));
		if(!file)
			return;
		std::string textureFileName;



		while(!file->eof())
		{
			std::string line = file->getLine();
			if(line.length() < 5)
				continue;

			if(line.substr(0, 5) == "page ")
			{
				textureFileName = line.substr(16);
				textureFileName = textureFileName.substr(0, textureFileName.length()-1);
			}
			if(line.substr(0, 5) == "char ")
			{
				std::vector<std::string> params = split(line.substr(5), " ");
				Glyph* glyph = new Glyph();
				glyph->id = 0;

				for(size_t i = 0; i < params.size(); i++)
				{
					std::string name = params[i].substr(0, params[i].find("="));
							if(name == "id")			glyph->id = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "x")			glyph->x = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "y")			glyph->y = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "width")		glyph->width = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "height")		glyph->height = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "xoffset")		glyph->xoffset = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "yoffset")		glyph->yoffset = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "xadvance")		glyph->xadvance = atoi(params[i].substr(name.length()+1).c_str());
					else if(name == "page" || name == "chnl") {}
					else
						Log::out<<"Didn't parse "<<name<<Log::newline;
				}

				if(glyph->id != 0)
					charmap[glyph->id] = glyph;
				else
					delete glyph;
			}
		}

		Log::out<<"Loaded font "<<fileName<<", "<<charmap.size()<<" glyphs"<<Log::newline;

		texture = resourceManager->getResource<Texture>("assets/fonts/"+textureFileName);
		delete file;
	}

	Font::~Font()
	{
		blib::ResourceManager::getInstance().dispose(texture);
		for(std::map<char, Glyph*>::iterator it = charmap.begin(); it != charmap.end(); it++)
			delete it->second;
		charmap.clear();
	}


	
	float Font::textlen(std::string text)
	{
		float scale = 1;//0.00075f;

		float posX = 0;

		for(size_t i = 0; i < text.size(); i++)
		{
			if(charmap.find(text[i]) == charmap.end())
				continue;
			Glyph* g = charmap[text[i]];
			posX += g->xadvance * scale;

		}
		return posX;
	}

	const Glyph* Font::getGlyph( const char &character ) const
	{
		return charmap.find(character)->second;
	}

}

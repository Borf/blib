#include <blib/Font.h>
#include <blib/Texture.h>
#include <blib/util/FileSystem.h>
#include <blib/util/StreamReader.h>
#include <blib/util/Log.h>

using blib::util::Log;

#include <vector>

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

namespace blib
{
	std::map<std::string, Font*> Font::fonts;

	Font* Font::getFontInstance(std::string name)
	{
		std::map<std::string, Font*>::iterator it = fonts.find(name);
		if(it == fonts.end())
		{
			Font* font = new Font("assets/fonts/"+name+".fnt");
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
	}


	std::vector<std::string> split(std::string value, std::string seperator)
	{
		std::vector<std::string> ret;
		while(value.find(seperator) != std::string::npos)
		{
			int index = value.find(seperator);
			if(index != 0)
				ret.push_back(value.substr(0, index));
			value = value.substr(index+seperator.length());
		}
		ret.push_back(value);
		return ret;
	}



	Font::Font(std::string fileName)
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

		texture = Texture::loadCached("assets/fonts/"+textureFileName);
		delete file;
	}

	Font::~Font()
	{
		for(std::map<char, Glyph*>::iterator it = charmap.begin(); it != charmap.end(); it++)
			delete it->second;
		charmap.clear();
	}



	/*
	void Font::render(std::string text, float scale)
	{
		if(text.empty())
			return;




		int posX = 0;

		std::vector<float> verts;
		verts.reserve(text.size() * 5 * 6);

		for(size_t i = 0; i < text.size(); i++)
		{
			if(charmap.find(text[i]) == charmap.end())
				continue;
			Glyph* g = charmap[text[i]];

			float tx = g->x /512.0f;
			float ty = g->y /512.0f;
			float tw = g->width /512.0f;
			float th = g->height /512.0f;


			const GLfloat gTopLeftRect[] = {
					scale*(posX + g->xoffset + 0.0f), 		scale*(0 + g->yoffset + g->height), 	tx, ty+th,
					scale*(posX + g->xoffset + 0.0f), 		scale*(0 + g->yoffset + 0.0f),			tx, ty,
					scale*(posX + g->xoffset + g->width), 	scale*(0 + g->yoffset + 0.0f), 			tx+tw, ty,

					scale*(posX + g->xoffset + g->width), 	scale*(0 + g->yoffset + 0.0f), 			tx+tw, ty,
					scale*(posX + g->xoffset + g->width), 	scale*(0 + g->yoffset + g->height), 	tx+tw, ty+th,
					scale*(posX + g->xoffset + 0.0f), 		scale*(0 + g->yoffset + g->height), 	tx, ty+th,
			};

			for(int i = 0; i < 4*6; i++)
				verts.push_back(gTopLeftRect[i]);

			posX += g->xadvance;
		}
		glBindTexture(GL_TEXTURE_2D, texture->texid);
		//shader->setColor(glm::vec4(1,1,1,1));

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), &verts[0]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GL_FLOAT), &verts[2]);
		glDrawArrays(GL_TRIANGLES, 0, verts.size()/4);

	}*/


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
}

#include <blib/gl/TextureMap.h>

#include <blib/config.h>
#include <blib/util/stb_image.h>
#include <blib/util/FileSystem.h>
#include <blib/config.h>
#include <blib/util/Log.h>
using blib::util::Log;

namespace blib
{
	namespace gl
	{


		TextureMap::TextureMap(int w, int h)
		{
			name = "Texturemap";
			texid = 0;
			width = w;
			height = h;
#ifdef BLIB_IOS
//            width = 1024;
//            height = 1024;
#endif
			taken = new bool[width/32*height/32];
			for(int i = 0; i < width/32*height/32; i++)
				taken[i] = false;
		}


		TextureMap::~TextureMap()
		{
			for (auto t : toLoad)
				stbi_image_free(t->data);
			assert(taken);
			delete[] taken;
			glDeleteTextures(1, &texid);
			for (auto i : info)
				delete i.second;
		}

		bool &TextureMap::isTaken(int x, int y)
		{
			static bool ROBINEETGRAAGKOEKJES = true;
			ROBINEETGRAAGKOEKJES = true;
			if(x >= width/32 || x < 0 || y >= height/32 || y < 0)
				return ROBINEETGRAAGKOEKJES;
			return taken[x + (width/32)*y];
		}



		blib::TextureMap::TexInfo* TextureMap::addTexture( std::string filename )
		{
			if(info.find(filename) != info.end())
				return info[filename];

			char* fileData = NULL;
			int length = blib::util::FileSystem::getData(filename, fileData);
			if(length < 0)
			{
				Log::err<<"Error loading texture "<<filename<<Log::newline;
				return NULL;
			}

			int depth = 4;
			int w,h;
			unsigned char* data = stbi_load_from_memory((stbi_uc*)fileData, length, &w, &h, &depth, 4);
			delete[] fileData;
			if(data == NULL)
			{
				Log::err<<"Error loading texture "<<filename<<", invalid texture file"<<Log::newline;
				Log::err << stbi_failure_reason() << Log::newline;
				return NULL;
			}


			if(w%32 != 0)
				Log::err<<"Error loading texture "<<filename<<", width is not a multiple of 32"<<Log::newline;
			if(h%32 != 0)
				Log::err<<"Error loading texture "<<filename<<", height is not a multiple of 32"<<Log::newline;

			int ww = (int)ceil(w / 32.0f);
			int hh = (int)ceil(h / 32.0f);

			int xoffset = 0;
			int yoffset = 0;

			bool ok = true;
			while(true)
			{
				ok = true;
				for(int x = 0; x < ww && ok; x++)
					for(int y = 0; y < hh && ok; y++)
						if(isTaken(xoffset+x, yoffset+y))
							ok = false;
				if(ok)
					break;
				else
				{
					xoffset++;
					if(xoffset >= width/32)
					{
						xoffset = 0;
						yoffset++;
					}
					if(yoffset >= height/32)
					{
						Log::out<<"No more room on texture map"<<Log::newline;
						Log::out << "Loading texture " << filename << Log::newline;
						return NULL;
					}
				}
			}

			for(int x = 0; x < ww; x++)
				for(int y = 0; y < hh; y++)
					isTaken(xoffset+x, yoffset+y) = true;


			TexInfo* newInfo = new TexInfo(this);
			newInfo->t1 = glm::vec2((xoffset*32+0.5f) / (float)width, (yoffset*32+0.5f) / (float)height);
			newInfo->t2 = newInfo->t1 + glm::vec2((w-1)/(float)width, (h-1)/(float)height);
			newInfo->data = data;
			newInfo->width = w;
			newInfo->height = h;
			newInfo->x = xoffset;
			newInfo->y = yoffset;

			newInfo->center = glm::vec2(w/2.0f, h/2.0f);


			toLoad.push_back(newInfo);


			info[filename] = newInfo;


			return newInfo;
		}

		void TextureMap::use()
		{
			if(texid == 0)
			{
				glGenTextures(1, &texid);
				glBindTexture(GL_TEXTURE_2D, texid);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			}

			glBindTexture(GL_TEXTURE_2D, texid);

			if(!toLoad.empty())
			{
				for(std::list<TexInfo*>::iterator it = toLoad.begin(); it != toLoad.end(); it++)
				{
					TexInfo* newData = *it;
					glTexSubImage2D(GL_TEXTURE_2D, 0, newData->x*32, newData->y*32, newData->width, newData->height, GL_RGBA, GL_UNSIGNED_BYTE, newData->data);
					stbi_image_free(newData->data);
				}
				toLoad.clear();
			}
		}


		void TextureMap::save(std::string filename)
		{
			//TODO: fix fopen here
		#if !defined(BLIB_ANDROID) && !defined(BLIB_IOS) && !defined(BLIB_NX)
			glBindTexture(GL_TEXTURE_2D, texid);
			char* pixels = new char[width*height*4];
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			FILE* shot;
			if((shot=fopen(filename.c_str(), "wb"))!=NULL)
			{
				int screenStats[4];
				glGetIntegerv(GL_VIEWPORT, screenStats);
				unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

				unsigned char Header[6]={((unsigned char)(width%256)),((unsigned char)(width/256)),((unsigned char)(height%256)),((unsigned char)(height/256)),32,0};
				fwrite(TGAheader, sizeof(unsigned char), 12, shot);
				fwrite(Header, sizeof(unsigned char), 6, shot);

				for(int y = 0; y < width; y++)
					for(int x = 0; x < height; x++)
						fwrite(pixels+4*(x+y*width), sizeof(unsigned char), 4, shot);
				fclose(shot);
			}
			delete [] pixels;
		#endif
		}

	}
}

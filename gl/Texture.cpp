#define _CRT_SECURE_NO_WARNINGS
#include "Texture.h"

#include <blib/util/Log.h>
using blib::util::Log;
#include <blib/util/FileSystem.h>
#include <blib/util/stb_image.h>
#include <string.h>


namespace blib
{
	namespace gl
	{
		std::map<std::string, Texture*> Texture::textureCache;

		Texture* Texture::loadCached(std::string fileName, int loadOptions)
		{
			if(textureCache.find(fileName) == textureCache.end())
				textureCache[fileName] = new Texture(fileName, loadOptions);
			return textureCache[fileName];
		}


		void Texture::clearCache()
		{
			for(std::map<std::string, Texture*>::iterator it = textureCache.begin(); it != textureCache.end(); it++)
			{
				delete it->second;
			}
			textureCache.clear();
		}


		Texture::Texture()
		{
			this->texid = 0;
			this->data = NULL;
		}

		Texture::Texture(std::string fileName, int loadOptions)
		{
			this->texid = 0;
			this->data = NULL;
			fromFile(fileName, loadOptions);
		}


		Texture::Texture(unsigned char* data, int width, int height)
		{
			this->texid = 0;
			this->data = NULL;
			fromData(data, width, height);
		}





		void Texture::fromFile(std::string fileName, int loadOptions)
		{
			char* fileData = NULL;
			int length = blib::util::FileSystem::getData(fileName, fileData);
			if(length == -1)
			{
				Log::err<<"Error loading texture "<<fileName<<Log::newline;
				return;
			}

			int depth;
			unsigned char* tmpData = stbi_load_from_memory((stbi_uc*)fileData, length, &originalWidth, &originalHeight, &depth, 0);
			delete[] fileData;
			data = new unsigned char[originalWidth*originalHeight*depth];
			memcpy(data, tmpData, originalWidth*originalHeight*depth);
			stbi_image_free(tmpData);

			if(data)
			{
				width = originalWidth;
				height = originalHeight;
				//make 4 bits
				if(depth == 3)
				{
					unsigned char* newData = new unsigned char[width*height*4];
					for(int y = 0; y < height; y++)
					{
						for(int x = 0; x < width; x++)
						{
							newData[4*(x+width*y)+0] = data[3*(x+width*y)+0];
							newData[4*(x+width*y)+1] = data[3*(x+width*y)+1];
							newData[4*(x+width*y)+2] = data[3*(x+width*y)+2];
							newData[4*(x+width*y)+3] = 255;
						}
					}
					delete[] data;
					data = newData;
					depth = 4;
				}


				//TODO: resize texture to power of 2 texture

				//fix transparency
				for(int y = 0; y < height; y++)
				{
					for(int x = 0; x < width; x++)
					{
						if(data[4*(x+width*y)+0] > 250 && data[4*(x+width*y)+1] < 5 && data[4*(x+width*y)+2] > 250)
						{
							int totalr = 0;
							int totalg = 0;
							int totalb = 0;
							int total = 0;
							for(int xx = -1; xx <= 1; xx++)
							{
								for(int yy = -1; yy <= 1; yy++)
								{
									int xxx = x+xx;
									int yyy = y+yy;
									if(xxx < 0 || xxx >= width || yyy < 0 || yyy >= height)
										continue;
									if(data[4*(xxx+width*yyy)+0] > 250 && data[4*(xxx+width*yyy)+1] < 5 && data[4*(xxx+width*yyy)+2] > 250)
										continue;
									if(data[4*(xxx+width*yyy)+3] == 0)
										continue;
									totalr += data[4*(xxx+width*yyy)+0];
									totalg += data[4*(xxx+width*yyy)+1];
									totalb += data[4*(xxx+width*yyy)+2];
									total++;
								}
							}
							if(total > 0)
							{
								data[4*(x+width*y)+0] = totalr / total;
								data[4*(x+width*y)+1] = totalg / total;
								data[4*(x+width*y)+2] = totalb / total;
							}
							data[4*(x+width*y)+3] = 0;
						}
					}
				}

				glGenTextures(1, &texid);
				glBindTexture(GL_TEXTURE_2D, texid);		
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

				if((loadOptions & TextureWrap) == 0)
				{
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				}

				if((loadOptions & KeepData) == 0)
				{
					delete[] data;
					data = NULL;
				}

				center = glm::vec2(originalWidth/2.0f, originalHeight/2.0f);
			}
		}



		void Texture::fromData(unsigned char* data, int width, int height)
		{
			this->width = this->originalWidth = width;
			this->height = this->originalHeight = height;


			/*
			FILE * shot;
			char buf[100];
			sprintf(buf, "Shot%i.tga", rand());
			if((shot=fopen(buf, "wb"))!=NULL)
			{
				unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

				int xfrom = 0;
				int yfrom = 0;
				int xto = width;
				int yto = height;

				int w= xto - xfrom;
				int h= yto - yfrom;

				unsigned char Header[6]={((int)(w%256)),((int)(w/256)),((int)(h%256)),((int)(h/256)),24,0};
				fwrite(TGAheader, sizeof(unsigned char), 12, shot);
				fwrite(Header, sizeof(unsigned char), 6, shot);

				for(int y = yfrom; y < yto; y++)
				{
					for(int x = xfrom; x < xto; x++)
					{
						fwrite(data+4*(x+y*width), sizeof(unsigned char), 3, shot);
					}
				}
				fclose(shot);
			}*/


			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			center = glm::vec2(originalWidth/2.0f, originalHeight/2.0f);
		}


		Texture::~Texture(void)
		{
			if(data != NULL)
				delete[] data;
			data = NULL;
			if(texid != 0)
				glDeleteTextures(1, &texid);
			texid = 0;
		}













		TextureMap::TextureMap()
		{
			width = 2048;
			height = 2048;
			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

			taken = new bool[width/32*height/32];
			for(int i = 0; i < width/32*height/32; i++)
				taken[i] = false;
		}


		TextureMap::~TextureMap()
		{
			assert(taken);
			delete taken;
			glDeleteTextures(1, &texid);
		}

		bool &TextureMap::isTaken(int x, int y)
		{
			static bool ROBINEETGRAAGKOEKJES = true;
			ROBINEETGRAAGKOEKJES = true;
			if(x >= width/32 || x < 0 || y >= height/32 || y < 0)
				return ROBINEETGRAAGKOEKJES;
			return taken[x + (width/32)*y];
		}



		TextureMap::TexInfo* TextureMap::addTexture( std::string filename )
		{
			if(info.find(filename) != info.end())
				return info[filename];
	
			glBindTexture(GL_TEXTURE_2D, texid);
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

			if(w%32 != 0)
				Log::err<<"Error loading texture "<<filename<<", width is not a multiple of 32"<<Log::newline;
			if(h%32 != 0)
				Log::err<<"Error loading texture "<<filename<<", height is not a multiple of 32"<<Log::newline;

			int xoffset = 0;
			int yoffset = 0;

			bool ok = true;
			while(true)
			{
				ok = true;
				for(int x = 0; x < w / 32 && ok; x++)
					for(int y = 0; y < h / 32 && ok; y++)
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
						return NULL;
				}
			}

			for(int x = 0; x < w / 32; x++)
				for(int y = 0; y < h / 32; y++)
					isTaken(xoffset+x, yoffset+y) = true;

			glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset*32, yoffset*32, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

			TexInfo* newInfo = new TexInfo(this);
			newInfo->t1 = glm::vec2((xoffset*32+0.5f) / (float)width, (yoffset*32+0.5f) / (float)height);
			newInfo->t2 = newInfo->t1 + glm::vec2((w-1)/(float)width, (h-1)/(float)height);

			newInfo->width = w;
			newInfo->height = h;

			info[filename] = newInfo;


			stbi_image_free(data);
			return newInfo;
		}


		void TextureMap::save(std::string filename)
		{
			//TODO: fix fopen here
		#ifndef ANDROID
			glBindTexture(GL_TEXTURE_2D, texid);
			char* pixels = new char[width*height*4];
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			FILE* shot;
			if((shot=fopen(filename.c_str(), "wb"))!=NULL)
			{
				int screenStats[4];
				glGetIntegerv(GL_VIEWPORT, screenStats);
				unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};

				unsigned char Header[6]={((int)(width%256)),((int)(width/256)),((int)(height%256)),((int)(height/256)),32,0};
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





		TextureMap::TexInfo::TexInfo(TextureMap* texMap)
		{
			this->texMap = texMap;
		}

	}
}

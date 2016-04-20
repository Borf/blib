#include "Texture.h"

#include <blib/util/Log.h>
using blib::util::Log;
#include <blib/util/FileSystem.h>
#include <blib/util/stb_image.h>
#include <string.h>

#include <blib/Texture.h>
#include <blib/SpriteSheet.h>

namespace blib
{
	/*std::map<std::string, blib::Texture*> Texture::textureCache;
	std::map<blib::Texture*, int> Texture::textureUseCount;



	void Texture::clearCache()
	{
		for(std::map<std::string, Texture*>::iterator it = textureCache.begin(); it != textureCache.end(); it++)
		{
			delete it->second;
		}
		textureCache.clear();
	}

	void Texture::unload()
	{
		textureUseCount[this]--;
		
	}*/

    
	namespace gl
	{

		template <class T>
		Texture<T>::Texture()
		{
            compressed = false;
			this->texid = 0;
			this->data = NULL;
		}

		template <class T>
		Texture<T>::Texture(std::string fileName, int loadOptions)
		{
            compressed = false;
			this->name = "Texture: " + fileName;
			this->texid = 0;
			this->data = NULL;
			this->fileName = fileName;
			if ((loadOptions & T::LoadLater) == 0)
				fromFile(fileName, loadOptions);
		}


		template <class T>
		Texture<T>::Texture(unsigned char* data, int width, int height)
		{
            compressed = false;
			this->texid = 0;
			this->data = data;
			fromData(data, width, height);
		}


		template <class T>
		void Texture<T>::fromFile(std::string fileName, int loadOptions)
		{
			T::loaded = false;
			char* fileData = NULL;
            int length = 0;
            compressed = false;
            
#ifdef BLIB_IOS
            if(blib::util::FileSystem::exists(fileName + ".pvrtc"))
            {
               // Log::out<<"Loading texture '"<<fileName<<".pvrtc'"<<Log::newline;
                compressed = true;
                compressedLength = blib::util::FileSystem::getData(fileName + ".pvrtc", fileData);
                data = (unsigned char*)fileData;
                
                T::originalWidth = sqrt(compressedLength*2);
                T::originalHeight = T::originalWidth;
                T::width = T::originalWidth;
                T::height = T::originalWidth;
                //Log::out<<T::originalWidth<<Log::newline;
                T::loaded = true;
                return;
            }
            else
#endif
            {
                length = blib::util::FileSystem::getData(fileName, fileData);
            }
            if(length <= 0)
            {
                Log::err<<"Error loading texture '"<<fileName<<"'"<<Log::newline;
                return;
            }
            
			int depth;
			int &_originalWidth = T::originalWidth;
			int &_originalHeight = T::originalHeight;

            _originalWidth = 10;
            _originalHeight = 10;
            depth = 1;
            
			unsigned char* tmpData = stbi_load_from_memory((stbi_uc*)fileData, length, &_originalWidth, &_originalHeight, &depth, 0);
			if (!tmpData)
			{
				const char* err = stbi_failure_reason();
				Log::out << "Error loading file " << fileName << Log::newline;
				Log::out << err << Log::newline;
				return;
			}
			delete[] fileData;
			data = new unsigned char[T::originalWidth*T::originalHeight*depth];
			memcpy(data, tmpData, T::originalWidth*T::originalHeight*depth);
			stbi_image_free(tmpData);

			if(data)
			{
				T::width = T::originalWidth;
				T::height = T::originalHeight;
				//make 4 bits
				if(depth == 3)
				{
					unsigned char* newData = new unsigned char[T::width*T::height*4];
					for(int y = 0; y < T::height; y++)
					{
						for(int x = 0; x < T::width; x++)
						{
							newData[4*(x+T::width*y)+0] = data[3*(x+T::width*y)+0];
							newData[4*(x+T::width*y)+1] = data[3*(x+T::width*y)+1];
							newData[4*(x+T::width*y)+2] = data[3*(x+T::width*y)+2];
							newData[4*(x+T::width*y)+3] = 255;
						}
					}
					delete[] data;
					data = newData;
					depth = 4;
				}


				//TODO: resize texture to power of 2 texture

				//fix transparency
				for(int y = 0; y < T::height; y++)
				{
					for(int x = 0; x < T::width; x++)
					{
						if(data[4*(x+T::width*y)+0] > 250 && data[4*(x+T::width*y)+1] < 5 && data[4*(x+T::width*y)+2] > 250)
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
									if(xxx < 0 || xxx >= T::width || yyy < 0 || yyy >= T::height)
										continue;
									if(data[4*(xxx+T::width*yyy)+0] > 250 && data[4*(xxx+T::width*yyy)+1] < 5 && data[4*(xxx+T::width*yyy)+2] > 250)
										continue;
									if(data[4*(xxx+T::width*yyy)+3] == 0)
										continue;
									totalr += data[4*(xxx+T::width*yyy)+0];
									totalg += data[4*(xxx+T::width*yyy)+1];
									totalb += data[4*(xxx+T::width*yyy)+2];
									total++;
								}
							}
							if(total > 0)
							{
								data[4*(x+T::width*y)+0] = totalr / total;
								data[4*(x+T::width*y)+1] = totalg / total;
								data[4*(x+T::width*y)+2] = totalb / total;
							}
							data[4*(x+T::width*y)+3] = 0;
						}
					}
				}
				T::center = glm::vec2(T::originalWidth/2.0f, T::originalHeight/2.0f);
			}
			//printf("Loaded image: %s -> %d x %d, resized to %d x %d\n", fileName.c_str(), _originalWidth, _originalHeight, T::width, T::height);
			T::loaded = true;
		}



		template <class T>
		void Texture<T>::fromData(unsigned char* data, int width, int height)
		{
			T::loaded = false;
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
			T::center = glm::vec2(T::originalWidth/2.0f, T::originalHeight/2.0f);
			T::loaded = true;
		}


		template <class T>
		Texture<T>::~Texture(void)
		{
			if(data != NULL)
				delete[] data;
			data = NULL;
			if(texid != 0)
				glDeleteTextures(1, &texid);
			texid = 0;
		}

		template <class T>
		void Texture<T>::use()
		{
			if(texid == 0)
			{
				if (data == NULL && T::fileName != "")
					fromFile(T::fileName, 0);

				glGenTextures(1, &texid);
				glBindTexture(GL_TEXTURE_2D, texid);
                if(compressed)
                {
#ifdef BLIB_IOS
                    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, T::width, T::height, 0, compressedLength, data);
#else
                    throw "oops";
#endif
                }
                else
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, T::width, T::height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
				if (T::nearest)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}


				if (T::textureRepeat)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}


			/*	float aniso = 0.0f;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);*/

			/*	if((loadOptions & TextureWrap) == 0)
				{
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
				}*/

				//if((loadOptions & KeepData) == 0)
				if(data)
				{
					delete[] data;
					data = NULL;
				}
			}

			glBindTexture(GL_TEXTURE_2D, texid);
		}


		template <class T>
		void Texture<T>::setSubImage(int x, int y, int width, int height, char* data)
		{
			use();
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}












#if !defined(BLIB_ANDROID) && !defined(BLIB_IOS) && !defined(BLIB_EMSCRIPTEN)
		MultiTextureMap::~MultiTextureMap()
		{
		}
		MultiTextureMap::MultiTextureMap(int width, int height)
		{
			this->width = width;
			this->height = height;
			this->count = 0;
			this->maxCount = 64;
			texid = 0;
		}



		MultiTextureMap::TexInfo* MultiTextureMap::addTexture(std::string filename)
		{
			if(info.find(filename) != info.end())
				return info[filename];

			char* fileData = NULL;
			int length = blib::util::FileSystem::getData(filename, fileData);
			if(length <= 0)
			{
				Log::err<<"Error loading texture "<<filename<<Log::newline;
				return NULL;
			}

			int depth = 4;
			int w,h;
			unsigned char* data = stbi_load_from_memory((stbi_uc*)fileData, length, &w, &h, &depth, 4);
			delete[] fileData;

			if(w != width)
				Log::err<<"Error loading texture "<<filename<<", width is not a multiple of 32"<<Log::newline;
			if(h != height)
				Log::err<<"Error loading texture "<<filename<<", height is not a multiple of 32"<<Log::newline;


//			stbi_image_free(data);
			count++;

			TexInfo* texinfo = new TexInfo();
			texinfo->data = data;
			texinfo->filename = filename;
			texinfo->t1 = glm::vec2(0,0);
			texinfo->t2 = glm::vec2(1,1);
			texinfo->depth = count-1;
			texinfo->texMap = this;
			texinfo->height = height;
			texinfo->width = width;
			texinfo->x = 0;
			texinfo->y = 0;

			info[filename] = texinfo;
			return texinfo;
		}

		void MultiTextureMap::use()
		{
			if(texid == 0)
			{
				glGenTextures(1,&texid);
				glBindTexture(GL_TEXTURE_2D_ARRAY,texid);
				glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, maxCount);

				glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
			}
			glBindTexture(GL_TEXTURE_2D_ARRAY, texid);
			for(std::map<std::string, TexInfo*>::iterator it = info.begin(); it != info.end(); it++)
			{
				if(it->second->data)
				{
					glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, it->second->depth, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, it->second->data);
					stbi_image_free(it->second->data);
					it->second->data = NULL;

				}
			}
		}
#endif


		template class Texture<blib::Texture>;
		template class Texture<blib::SpriteSheet>;

	}
}

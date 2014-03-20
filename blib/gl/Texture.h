#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <blib/Texture.h>

namespace blib
{

	namespace gl
	{
		template<class T>
		class Texture : public T
		{
		public: //TODO: make protected
			Texture();
			void fromData(unsigned char* data, int width, int height);
			void fromFile(std::string fileName, int loadOptions);

			Texture(std::string fileName, int loadOptions = 0);
		public:

			unsigned char* data;

			GLuint texid;
			Texture(unsigned char* data, int width, int height);
			
			virtual void use();
			virtual void setSubImage(int x, int y, int width, int height, char* data);

		protected:
			virtual ~Texture(void);
		};
		
#ifndef ANDROID
		class MultiTextureMap : public blib::Texture
		{
		public:
			int maxCount;
			int count;
			int width, height;

			class TexInfo
			{
			public:
			//	TexInfo();
				MultiTextureMap* texMap;
				glm::vec2 t1;
				glm::vec2 t2;
				int x,y,width,height;

				int depth;
				std::string filename;

				unsigned char* data;
			};

			void use();
			GLuint texid;
			std::map<std::string, TexInfo*> info;

			MultiTextureMap(int width, int height);
			~MultiTextureMap();
			TexInfo* addTexture(std::string filename);


		};
#endif

	}
}



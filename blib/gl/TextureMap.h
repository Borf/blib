#pragma once

#include <blib/TextureMap.h>

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


#include <list>

namespace blib
{
	namespace gl
	{
		class TextureMap : public blib::TextureMap
		{
		public:
			class TexInfo : public blib::TextureMap::TexInfo
			{
			public:
				TexInfo(TextureMap* texMap) : blib::TextureMap::TexInfo(texMap)
				{
					data = NULL;
				}
				unsigned char* data;
			};


			GLuint texid;
			std::map<std::string, TexInfo*> info;

			std::list<TexInfo*> toLoad;


			bool* taken;
			inline bool &isTaken(int x, int y);
			void save(std::string filename);

			TextureMap();
			~TextureMap();
			blib::TextureMap::TexInfo* addTexture(std::string filename);
			void use();
		};

	}
}
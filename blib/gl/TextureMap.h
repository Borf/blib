#pragma once

#include <blib/TextureMap.h>

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <gl/glew.h>
#endif

namespace blib
{
	namespace gl
	{
		class TextureMap : public blib::TextureMap
		{
		public:
			GLuint texid;
			std::map<std::string, TexInfo*> info;


			bool* taken;
			inline bool &isTaken(int x, int y);
			void save(std::string filename);

			TextureMap();
			~TextureMap();
			TexInfo* addTexture(std::string filename);
			void use();		
		};

	}
}
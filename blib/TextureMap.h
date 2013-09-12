#pragma once

#include <blib/texture.h>

namespace blib
{
	class TextureMap : public Texture
	{
	public:
		class TexInfo
		{
		public:
			TexInfo(TextureMap* texMap)
			{
				this->texMap = texMap;
			}
			TextureMap* texMap;
			glm::vec2 t1;
			glm::vec2 t2;
			int x,y,width,height;
		};


		virtual void use() = 0;
		virtual void save(std::string filename) {};
		virtual TexInfo* addTexture(std::string filename) = NULL;
	};
}
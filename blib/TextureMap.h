#pragma once

#include <blib/Texture.h>

namespace blib
{
	class TextureMap : public Texture
	{
	public:
		class TexInfo
		{
		public:
			TexInfo(Texture* texMap)
			{
				this->texMap = texMap;
			}
			Texture* texMap;
			glm::vec2 t1; //uv
			glm::vec2 t2; //uv
			int x,y,width,height; // in texels
			glm::vec2 center;//center in texels
		};


		virtual const unsigned char* getData() const { return NULL; };
		virtual void use() = 0;
		virtual void save(std::string filename) {};
		virtual TexInfo* addTexture(std::string filename) = 0;
	};
}
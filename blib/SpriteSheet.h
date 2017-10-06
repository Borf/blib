#pragma once

#include "TextureMap.h"

namespace blib
{
	class SpriteSheet : public blib::Texture
	{
		blib::TextureMap::TexInfo* buildFrame(int x, int y);
	public:
		std::map<int, std::map<int, TextureMap::TexInfo*> > texMapCache;

		int spriteCountX;
		int spriteCountY;
		int frameCount;

		int spriteWidth;
		int spriteHeight;
		glm::vec2 spriteCenter;

		virtual TextureMap::TexInfo* addTexture(std::string filename) { throw "Cannot add to a spritesheet"; };


		virtual blib::TextureMap::TexInfo* getFrame(int x, int y);
	};
}
#include "SpriteSheet.h"

namespace blib
{
	TextureMap::TexInfo* SpriteSheet::getFrame( int x, int y )
	{
		assert(x >= 0 && x < spriteCountX && y >= 0 && y < spriteCountY);
		
		std::map<int, std::map<int, TextureMap::TexInfo*> >::iterator xIt = texMapCache.find(x);
		if(xIt != texMapCache.end())
		{
			std::map<int, TextureMap::TexInfo*>::iterator yIt = xIt->second.find(y);
			if(yIt != xIt->second.end())
				return yIt->second;
			else
			{
				TextureMap::TexInfo* info = buildFrame(x,y);
				texMapCache[x][y] = info;
				return info;
			}
		}
		else
		{
			TextureMap::TexInfo* info = buildFrame(x,y);
			texMapCache[x][y] = info;
			return info;
		}
		return NULL;
	}

	TextureMap::TexInfo* SpriteSheet::buildFrame( int x, int y )
	{
		TextureMap::TexInfo* info = new TextureMap::TexInfo(this);

		info->t1 = glm::vec2(x * (spriteWidth/(float)width), y*((float)spriteHeight/height));
		info->t2 = info->t1 + glm::vec2(spriteWidth/(float)width, (float)spriteHeight/height);
		info->center = glm::vec2(spriteWidth / 2.0f, spriteHeight / 2.0f);// info->widthspriteCenter / glm::vec2(spriteWidth, spriteHeight);
		info->width = spriteWidth;
		info->height = spriteHeight;
		info->x = x * spriteWidth;
		info->y = y * spriteHeight;
		return info;
	}

}

#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

namespace blib
{
	class Texture
	{
	public:
		virtual void use() = 0;

		int width;
		int height;
		int originalWidth;
		int originalHeight;

		glm::vec2 center;

		static std::map<std::string, Texture*> textureCache;
		static Texture* loadCached(std::string fileName, int loadOptions = 0);
		static void clearCache();

	};
}
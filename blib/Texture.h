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
		bool loaded;
		int width;
		int height;
		int originalWidth;
		int originalHeight;

		glm::vec2 center;

		static std::map<std::string, blib::Texture*> textureCache;
		
		template<class T>
		static T* loadCached(std::string fileName, int loadOptions = 0)
		{
			if(textureCache.find(fileName) == textureCache.end())
				textureCache[fileName] = new T(fileName, loadOptions);
			return (T*)textureCache[fileName];
		}
		static void clearCache();

	};
}
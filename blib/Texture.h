#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

namespace blib
{
	class Texture
	{
	public:
		enum LoadOptions
		{
			NoResize = 1,
			KeepData = 2,
			LoadLater = 4,
			TextureWrap = 8,
		};


		virtual void use() = 0;
		virtual void setSubImage(int x, int y, int width, int height, char* data)  {};
		bool loaded;
		int width;
		int height;
		int originalWidth;
		int originalHeight;
		std::string fileName;

		glm::vec2 center;

		static std::map<blib::Texture*, int> textureUseCount;
		static std::map<std::string, blib::Texture*> textureCache;
		
		template<class T>
		static T* loadCached(std::string fileName, int loadOptions = 0)
		{
			if(textureCache.find(fileName) == textureCache.end())
				textureCache[fileName] = new T(fileName, loadOptions);
			textureUseCount[textureCache[fileName]]++;
			return (T*)textureCache[fileName];
		}
		virtual void unload();
		static void clearCache();


	protected:
		virtual ~Texture() {};


	};
}
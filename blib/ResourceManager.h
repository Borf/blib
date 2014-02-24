#pragma once

#include <blib/util/FastDelegate.h>
#include <string>
#include <blib/VBO.h>

namespace blib
{
	class Resource;
	class Texture;
	class VBO;
	class Renderer;
	class Shader;
	class TextureMap;
	class FBO;
	class Font;
	class SpriteSheet;

	class ResourceManager
	{
	public:
		ResourceManager()
		{
			renderer = NULL;
			texture = NULL;
		}
		template<class T>
		inline T* getResource(const std::string &name)
		{
			throw "Invalid resource";
			return NULL;
		}
		template<class T>
		inline T* getResource(int width, int height)
		{
			throw "Invalid resource";
			return NULL;
		}
		template<class T>
		inline T* getResource()
		{
			throw "Invalid resource";
			return NULL;
		}
		



		fastdelegate::FastDelegate0<Renderer*>							renderer;
		fastdelegate::FastDelegate1<const std::string &, Texture*>		texture;
		fastdelegate::FastDelegate2<int, int, Texture*>					emptyTexture;
		fastdelegate::FastDelegate1<const std::string &, SpriteSheet*>	spritesheet;
		fastdelegate::FastDelegate1<const std::string &, Shader*>		shader;
		fastdelegate::FastDelegate1<const std::string &, Font*>			font;
		fastdelegate::FastDelegate0<Shader*>							emptyshader;
		fastdelegate::FastDelegate0<VBO*>								vbo;
		fastdelegate::FastDelegate0<FBO*>								fbo;
		fastdelegate::FastDelegate0<TextureMap*>						texturemap;
	};

	template<>
	inline Renderer* ResourceManager::getResource<Renderer>()
	{
		return renderer();
	}
	template<>
	inline Texture* ResourceManager::getResource<Texture>(const std::string &name)
	{
		return texture(name);
	}
	template<>
	inline Texture* ResourceManager::getResource<Texture>(int width, int height)
	{
		return emptyTexture(width, height);
	}
	template<>
	inline SpriteSheet* ResourceManager::getResource<SpriteSheet>(const std::string &name)
	{
		return spritesheet(name);
	}
	template<>
	inline TextureMap* ResourceManager::getResource<TextureMap>()
	{
		return texturemap();
	}
	template<>
	inline Shader* ResourceManager::getResource<Shader>(const std::string &name)
	{
		return shader(name);
	}	

	template<>
	inline Font* ResourceManager::getResource<Font>(const std::string &name)
	{
		return font(name);
	}

	template<>
	inline Shader* ResourceManager::getResource<Shader>()
	{
		return emptyshader();
	}

	template<>
	inline VBO* ResourceManager::getResource<VBO>()
	{
		return vbo();
	}
	template<>
	inline FBO* ResourceManager::getResource<FBO>()
	{
		return fbo();
	}


	class NullResource : public ResourceManager
	{
	};

}

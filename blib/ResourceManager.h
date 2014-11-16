#pragma once

#include <functional>
#include <string>
#include <blib/VBO.h>

namespace blib
{
	class Resource;
	class Texture;
	class Renderer;
	class Shader;
	class TextureMap;
	class VBO;
	class FBO;
	class VIO;
	class Font;
	class SpriteSheet;

	class ResourceManager
	{
	public:
		ResourceManager()
		{
//			renderer = NULL;
//			texture = NULL;
		}
		template<class T>
		inline T* getResource(const std::string &name)
		{
			throw "Invalid resource";
			return NULL;
		}
		template<class T>
		inline T* getResource(int options, const std::string &name)
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
		

		std::function<Renderer*()>										renderer;
		std::function<Texture*(const std::string &, int)>				texture;
		std::function<Texture*(int, int)>								emptyTexture;
		std::function<SpriteSheet*(const std::string &)>				spritesheet;
		std::function<Shader*(const std::string &)>						shader;
		std::function<Font*(const std::string &)>						font;
		std::function<Shader*()>										emptyshader;
		std::function<VBO*()>											vbo;
		std::function<VIO*()>											vio;
		std::function<FBO*()>											fbo;
		std::function<TextureMap*()>									texturemap;

	};

	template<>
	inline Renderer* ResourceManager::getResource<Renderer>()
	{
		return renderer();
	}
	template<>
	inline Texture* ResourceManager::getResource<Texture>(int loadOptions, const std::string &name)
	{
		return texture(name, loadOptions);
	}
	template<>
	inline Texture* ResourceManager::getResource<Texture>(const std::string &name)
	{
		return texture(name, 0);
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
	inline VIO* ResourceManager::getResource<VIO>()
	{
		return vio();
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

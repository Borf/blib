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
		inline T* getResource()
		{
			throw "Invalid resource";
			return NULL;
		}
		



		fastdelegate::FastDelegate0<Renderer*>						renderer;
		fastdelegate::FastDelegate1<const std::string &, Texture*>	texture;
		fastdelegate::FastDelegate1<const std::string &, Shader*>	shader;
		fastdelegate::FastDelegate0<Shader*>						emptyshader;
		fastdelegate::FastDelegate0<VBO*>							vbo;
		fastdelegate::FastDelegate0<FBO*>							fbo;
		fastdelegate::FastDelegate0<TextureMap*>					texturemap;
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
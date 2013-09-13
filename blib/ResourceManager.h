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
		


		template<>
		inline Renderer* getResource<Renderer>()
		{
			return renderer();
		}
		template<>
		inline Texture* getResource<Texture>(const std::string &name)
		{
			return texture(name);
		}
		template<>
		inline TextureMap* getResource<TextureMap>()
		{
			return texturemap();
		}
		template<>
		inline Shader* getResource<Shader>(const std::string &name)
		{
			return shader(name);
		}	

		template<>
		inline VBO* getResource<VBO>()
		{
			return vbo();
		}





		fastdelegate::FastDelegate0<Renderer*>	renderer;
		fastdelegate::FastDelegate1<const std::string &, Texture*>	texture;
		fastdelegate::FastDelegate1<const std::string &, Shader*>	shader;
		fastdelegate::FastDelegate0<VBO*>		vbo;
		fastdelegate::FastDelegate0<TextureMap*>		texturemap;
	};


	class NullResource : public ResourceManager
	{
	};

}
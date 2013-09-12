#pragma once

#include <blib/ResourceManager.h>


namespace blib
{
	namespace gl
	{
		class ResourceManager : public blib::ResourceManager
		{
		public:
			ResourceManager();

			Renderer* getRenderer();
			Texture* getTexture(const std::string &name);
			Shader* getShader(const std::string &name);
			TextureMap* getTextureMap();

		};


	}
}
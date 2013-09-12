#include "ResourceManager.h"

#include <blib/gl/Texture.h>
#include <blib/gl/TextureMap.h>
#include <blib/gl/Renderer.h>
#include <blib/gl/Shader.h>


namespace blib
{
	namespace gl
	{


		ResourceManager::ResourceManager()
		{
			renderer = fastdelegate::MakeDelegate(this, &ResourceManager::getRenderer);
			texture  = fastdelegate::MakeDelegate(this, &ResourceManager::getTexture);
			//	shader   = fastdelegate::MakeDelegate(this, &ResourceManager::getShader);
			//	vbo      = fastdelegate::MakeDelegate(this, &ResourceManager::getVbo);

			texturemap = fastdelegate::MakeDelegate(this, &ResourceManager::getTextureMap);
		}

		blib::Renderer* ResourceManager::getRenderer()
		{
			return new blib::gl::Renderer();
		}

		blib::Texture* ResourceManager::getTexture( const std::string &name )
		{
			return Texture::loadCached(name);
		}

		blib::Shader* ResourceManager::getShader( const std::string &name )
		{
			return Shader::fromFile<Shader>(name + ".vert", name + ".frag");
		}
		blib::TextureMap* ResourceManager::getTextureMap( )
		{
			return new TextureMap();
		}

	}
}
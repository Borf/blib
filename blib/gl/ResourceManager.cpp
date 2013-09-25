#include "ResourceManager.h"

#include <blib/gl/Texture.h>
#include <blib/gl/TextureMap.h>
#include <blib/gl/Renderer.h>
#include <blib/gl/Shader.h>
#include <blib/gl/FBO.h>


namespace blib
{
	namespace gl
	{


		ResourceManager::ResourceManager()
		{
			renderer	= fastdelegate::MakeDelegate(this, &ResourceManager::getRenderer);
			texture		= fastdelegate::MakeDelegate(this, &ResourceManager::getTexture);
			shader		= fastdelegate::MakeDelegate(this, &ResourceManager::getShader);
			emptyshader = fastdelegate::MakeDelegate(this, &ResourceManager::getShaderEmpty);
			//	vbo      = fastdelegate::MakeDelegate(this, &ResourceManager::getVbo);
			fbo			= fastdelegate::MakeDelegate(this, &ResourceManager::getFBO);

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
			return new Shader(name + ".vert", name + ".frag");
		}

		blib::Shader* ResourceManager::getShaderEmpty()
		{
			return new Shader();
		}

		blib::TextureMap* ResourceManager::getTextureMap( )
		{
			return new TextureMap();
		}

		blib::FBO* ResourceManager::getFBO()
		{
			return new FBO();
		}

	}
}
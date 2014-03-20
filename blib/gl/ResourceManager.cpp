#include "ResourceManager.h"

#include <blib/gl/Texture.h>
#include <blib/gl/TextureMap.h>
#include <blib/gl/Renderer.h>
#include <blib/gl/Shader.h>
#include <blib/gl/VBO.h>
#include <blib/gl/FBO.h>
#include <blib/SpriteSheet.h>
#include <blib/Font.h>

#include <json/json.h>
#include <blib/util/FileSystem.h>


namespace blib
{
	namespace gl
	{


		ResourceManager::ResourceManager()
		{
			renderer	= fastdelegate::MakeDelegate(this, &ResourceManager::getRenderer);
			texture		= fastdelegate::MakeDelegate(this, &ResourceManager::getTexture);
			emptyTexture= fastdelegate::MakeDelegate(this, &ResourceManager::getEmptyTexture);
			shader		= fastdelegate::MakeDelegate(this, &ResourceManager::getShader);
			emptyshader = fastdelegate::MakeDelegate(this, &ResourceManager::getShaderEmpty);
			vbo			= fastdelegate::MakeDelegate(this, &ResourceManager::getVBO);
			fbo			= fastdelegate::MakeDelegate(this, &ResourceManager::getFBO);

			spritesheet = fastdelegate::MakeDelegate(this, &ResourceManager::getSpriteSheet);

			texturemap = fastdelegate::MakeDelegate(this, &ResourceManager::getTextureMap);
			font		= fastdelegate::MakeDelegate(this, &ResourceManager::getFont);
		}

		blib::Renderer* ResourceManager::getRenderer()
		{
			return new blib::gl::Renderer();
		}

		blib::Texture* ResourceManager::getTexture( const std::string &name, int loadOptions )
		{
			return Texture<blib::Texture>::loadCached<Texture<blib::Texture> >(name, loadOptions);
		}
		
		blib::Texture* ResourceManager::getEmptyTexture( int width, int height )
		{
			return new blib::gl::Texture<blib::Texture>(NULL, width, height);

		}


		blib::Shader* ResourceManager::getShader( const std::string &name )
		{
			return new Shader(blib::util::FileSystem::getData(name + ".vert"), blib::util::FileSystem::getData(name + ".frag"));
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

		blib::VBO* ResourceManager::getVBO()
		{
			return new blib::gl::VBO();
		}


		blib::Font* ResourceManager::getFont(const std::string &name)
		{
			return Font::getFontInstance(name, this);
		}

		blib::SpriteSheet* ResourceManager::getSpriteSheet(const std::string &name)
		{
			Json::Value v = util::FileSystem::getJson(name + ".json");
			std::string directory = name;
			
			if(directory.find('/') != std::string::npos)
				directory = directory.substr(0, directory.rfind('/')+1);
			else if(directory.find('\\') != std::string::npos)
				directory = directory.substr(0, directory.rfind('\\')+1);


			blib::SpriteSheet* spriteSheet = blib::Texture::loadCached<blib::gl::Texture<blib::SpriteSheet> >(directory + v["tex"].asString());

			spriteSheet->spriteCountX = v["count"][0u].asInt();
			spriteSheet->spriteCountY = v["count"][1u].asInt();

			spriteSheet->spriteWidth = spriteSheet->originalWidth / v["count"][0u].asInt();
			spriteSheet->spriteHeight = spriteSheet->originalHeight / v["count"][1u].asInt();
			spriteSheet->spriteCenter = glm::vec2(spriteSheet->spriteWidth / 2, spriteSheet->spriteHeight/2);
			return spriteSheet;
		}

	}
}
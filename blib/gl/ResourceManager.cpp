#include "ResourceManager.h"

#include <blib/gl/Texture.h>
#include <blib/gl/TextureMap.h>
#include <blib/gl/Renderer.h>
#include <blib/gl/Shader.h>
#include <blib/gl/VBO.h>
#include <blib/gl/FBO.h>
#include <blib/gl/VIO.h>
#include <blib/SpriteSheet.h>
#include <blib/Font.h>

#include <blib/json.hpp>
#include <blib/util/FileSystem.h>


namespace blib
{
	namespace gl
	{
		ResourceManager::ResourceManager()
		{
			renderer	= std::bind(&ResourceManager::getRenderer ,this);
			texture		= std::bind(&ResourceManager::getTexture, this, std::placeholders::_1, std::placeholders::_2);
			emptyTexture =std::bind(&ResourceManager::getEmptyTexture, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			shader		= std::bind(&ResourceManager::getShader, this, std::placeholders::_1);
			emptyshader = std::bind(&ResourceManager::getShaderEmpty, this);
			vbo			= std::bind(&ResourceManager::getVBO, this);
			fbo			= std::bind(&ResourceManager::getFBO, this);
			vio			= std::bind(&ResourceManager::getVIO, this);

			spritesheet = std::bind(&ResourceManager::getSpriteSheet, this, std::placeholders::_1);

			texturemap =  std::bind(&ResourceManager::getTextureMap, this, std::placeholders::_1, std::placeholders::_2);
			font		= std::bind(&ResourceManager::getFont, this, std::placeholders::_1, std::placeholders::_2);
		}


		blib::Renderer* ResourceManager::getRenderer()
		{
			return new blib::gl::Renderer();
		}

		blib::Texture* ResourceManager::getTexture( const std::string &name, int loadOptions )
		{
            return new Texture<blib::Texture>(name, loadOptions);
		}
		
		blib::Texture* ResourceManager::getEmptyTexture( int width, int height, unsigned char* data)
		{
			return new blib::gl::Texture<blib::Texture>(data, width, height);

		}


		blib::Shader* ResourceManager::getShader( const std::string &name )
		{
#if defined(BLIB_GL_FULL)
			return new Shader(
				"//" + name + ".vert\n" + blib::util::FileSystem::getData("assets/shaders/gl/" + name + ".vert"), 
				"//" + name + ".frag\n" + blib::util::FileSystem::getData("assets/shaders/gl/" + name + ".frag"));
#elif defined(BLIB_GL_ES)
			return new Shader(blib::util::FileSystem::getData("assets/shaders/gles/" + name + ".vert"), blib::util::FileSystem::getData("assets/shaders/gles/" + name + ".frag"));
#else
#error	You need to define a GL version...
#endif
			
		}

		blib::Shader* ResourceManager::getShaderEmpty()
		{
			return new Shader();
		}

		blib::TextureMap* ResourceManager::getTextureMap(int width, int height )
		{
			return new TextureMap(width, height);
		}

		blib::FBO* ResourceManager::getFBO()
		{
			return new FBO();
		}

		blib::VBO* ResourceManager::getVBO()
		{
			return new blib::gl::VBO();
		}
		blib::VIO* ResourceManager::getVIO()
		{
			return new blib::gl::VIO();
		}


		blib::Font* ResourceManager::getFont(const std::string &name, float size)
		{
            return new Font("assets/fonts/" + name, this, size);
		}

		blib::SpriteSheet* ResourceManager::getSpriteSheet(const std::string &name)
		{
			json v = util::FileSystem::getJson(name + ".json");
			std::string directory = name;
			
			if(directory.find('/') != std::string::npos)
				directory = directory.substr(0, directory.rfind('/')+1);
			else if(directory.find('\\') != std::string::npos)
				directory = directory.substr(0, directory.rfind('\\')+1);


			blib::SpriteSheet* spriteSheet = new blib::gl::Texture<blib::SpriteSheet>(directory + v["tex"].get<std::string>());
			spriteSheet->spriteCountX = v["count"][0].get<int>();
			spriteSheet->spriteCountY = v["count"][1].get<int>();
			spriteSheet->frameCount = v.value("frames", spriteSheet->spriteCountX * spriteSheet->spriteCountY);


			spriteSheet->spriteWidth = spriteSheet->originalWidth / v["count"][0].get<int>();
			spriteSheet->spriteHeight = spriteSheet->originalHeight / v["count"][1].get<int>();
			spriteSheet->spriteCenter = glm::vec2(spriteSheet->spriteWidth / 2, spriteSheet->spriteHeight/2);
			return spriteSheet;
		}

	}
}
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

			blib::Renderer* getRenderer();
			SpriteSheet* getSpriteSheet(const std::string &name);
			Texture* getTexture(const std::string &name, int loadOptions);
			Texture* getEmptyTexture(int width, int height, unsigned char* data);
			Shader* getShader(const std::string &name);
			Font* getFont(const std::string &name, float size);
			Shader* getShaderEmpty();
			TextureMap* getTextureMap(int w, int h);
			FBO* getFBO();
			VBO* getVBO();
			VIO* getVIO();
		};


	}
}
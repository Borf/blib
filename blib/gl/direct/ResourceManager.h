#pragma once

#include <blib/ResourceManager.h>


namespace blib
{
	namespace gl
	{
		namespace direct
		{
			class ResourceManager : public blib::ResourceManager
			{
			public:
				ResourceManager();

				Renderer* getRenderer();
				SpriteSheet* getSpriteSheet(const std::string &name);
				Texture* getTexture(const std::string &name, int loadOptions);
				Texture* getEmptyTexture(int width, int height);
				Shader* getShader(const std::string &name);
				Font* getFont(const std::string &name);
				Shader* getShaderEmpty();
				TextureMap* getTextureMap();
				FBO* getFBO();
				VBO* getVBO();
			};
		}

	}
}
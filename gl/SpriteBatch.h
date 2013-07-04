#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Texture.h>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <queue>


namespace blib
{
	namespace gl
	{
		class Texture;
		class Shader;



		class SpriteBatch : public GlInitRegister
		{
#define MAX_SPRITES 100000
			class Shader : public blib::gl::Shader, GlResizeRegister
			{
			public:
				void resizeGl(int width, int height);
				void init();
			};

			int spriteCount;
			VertexPosition3Texture2 verts[MAX_SPRITES*4];
			unsigned short indices[MAX_SPRITES*6];

			
			bool active;
			int depth;

			blib::gl::Shader* shader;
		public:
			SpriteBatch();

			virtual void initGl();

			virtual void begin();
			virtual void end();

			virtual void draw(Texture* sprite, glm::mat4 transform);
			virtual void draw(TextureMap::TexInfo* sprite, glm::mat4 transform);

		};



	}
}

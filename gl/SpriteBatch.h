#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Texture.h>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <blib/gl/VBO.h>
#include <blib/gl/VIO.h>
#include <queue>



namespace blib
{
	namespace gl
	{
		class Texture;
		class Shader;



		class SpriteBatch : public GlInitRegister
		{
			typedef VertexPosition3Texture2 vertexDef;

#define MAX_SPRITES 200000
			class Shader : public blib::gl::Shader, GlResizeRegister
			{
			public:
				void resizeGl(int width, int height);
				void init();
			};

			VBO<vertexDef> vbo;
			vertexDef	verts[MAX_SPRITES];
			VIO<unsigned short> vio;

			int spriteCount;

			
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

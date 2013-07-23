#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Texture.h>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <blib/gl/VBO.h>
#include <blib/gl/VIO.h>
#include <blib/math/Rectangle.h>
#include <queue>



namespace blib
{
	namespace gl
	{
		class Texture;
		class Shader;



		class SpriteBatch : public GlInitRegister
		{
			typedef VertexPosition2Texture2Color4 vertexDef;

#define MAX_SPRITES 110000
			class Shader : public blib::gl::Shader, GlResizeRegister
			{
			public:
				void resizeGl(int width, int height);
				void init();
			};

			VBO<vertexDef> vbo;
			vertexDef	verts[MAX_SPRITES];
			VIO<unsigned short> vio;
			std::vector<std::pair<Texture*, unsigned short> > materialIndices;
			Texture* currentTexture;

			int spriteCount;
			bool active;
			int depth;

			glm::mat4 matrix;


			blib::gl::Shader* shader;
		public:
			SpriteBatch();

			virtual void initGl();

			virtual void begin(glm::mat4 matrix = glm::mat4());
			virtual void end();

			virtual void draw(Texture* sprite, glm::mat4 transform, glm::vec2 center = glm::vec2(0,0), blib::math::Rectangle src = blib::math::Rectangle(0,0,1,1));
			virtual void draw(TextureMap::TexInfo* sprite, glm::mat4 transform);

		};



	}
}

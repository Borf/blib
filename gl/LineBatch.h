#pragma once

#include <glm/glm.hpp>
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
		class Shader;

		class LineBatch : public GlInitRegister
		{
			typedef VertexPosition2Color4 vertexDef;

#define MAX_LINES 110000
			class Shader : public blib::gl::Shader, GlResizeRegister
			{
			public:
				void resizeGl(int width, int height);
				void init();
			};

			VBO<vertexDef> vbo;
			vertexDef	verts[MAX_LINES];
			VIO<unsigned short> vio;

			int lineCount;
			bool active;

			glm::mat4 matrix;
			blib::gl::Shader* shader;
		public:
			LineBatch();

			virtual void initGl();

			virtual void begin(glm::mat4 matrix = glm::mat4());
			virtual void end();

			virtual void draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color = glm::vec4(1,1,0,1), glm::mat4 transform = glm::mat4());

		};



	}
}

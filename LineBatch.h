#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <queue>

namespace blib
{
	namespace gl { class Shader; }
	class Renderer;
	class IDrawableLine;

	class LineBatch : public gl::GlInitRegister
	{
		typedef VertexP2C4 vertexDef;

#define MAX_LINES 10000
		class Shader : public blib::gl::Shader, gl::GlResizeRegister
		{
		public:
			void resizeGl(int width, int height);
			void init();
		};

		Renderer*	renderer;
		vertexDef	verts[MAX_LINES];

		int lineCount;
		bool active;

		glm::mat4 matrix;
	public:
		LineBatch(Renderer* renderer);

		blib::Shader* shader;

		virtual void initGl();

		virtual void begin(glm::mat4 matrix = glm::mat4());
		virtual void end();

		virtual void draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color = glm::vec4(1,1,0,1), glm::mat4 transform = glm::mat4());
		virtual void draw(blib::IDrawableLine &drawable, glm::vec4 color = glm::vec4(1,1,0,1), bool drawNormal = false, glm::mat4 transform = glm::mat4());

	};

}

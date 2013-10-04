#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/gl/Shader.h>
#include <blib/RenderState.h>
#include <queue>

namespace blib
{
	namespace gl { class Shader; }
	class Renderer;
	class IDrawableLine;
	class ResourceManager;

	class LineBatch : public gl::GlResizeRegister
	{
		typedef VertexP2C4 vertexDef;

#define MAX_LINES 40000


		Renderer*	renderer;
		vertexDef	verts[MAX_LINES];

		int lineCount;
		bool active;

	public:
		LineBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState = RenderState());

		RenderState renderState;
		blib::Shader* shader;
		glm::mat4 matrix;			//TODO: make private

		virtual void resizeGl(int width, int height);

		virtual void begin(glm::mat4 matrix = glm::mat4());
		virtual void end();

		virtual void draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color = glm::vec4(1,1,0,1), glm::mat4 transform = glm::mat4());
		virtual void draw(const blib::IDrawableLine &drawable, glm::vec4 color = glm::vec4(1,1,0,1), bool drawNormal = false, glm::mat4 transform = glm::mat4());

	};

}

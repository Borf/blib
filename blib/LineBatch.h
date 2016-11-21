#pragma once

#include <glm/glm.hpp>
#include <blib/gl/Vertex.h>
#include <blib/gl/GlResizeRegister.h>
#include <blib/RenderState.h>
#include <queue>

namespace blib
{
	class Shader;
	class Renderer;
	class IDrawableLine;
	class ResourceManager;
	class VBO;

	class LineBatch : public gl::GlResizeRegister
	{
		typedef VertexP2C4 vertexDef;
	public:
		class Cache
		{
		public:
			std::vector<vertexDef> verts;
			~Cache();
		};
	private:
		Renderer*	renderer;
		std::vector<vertexDef>	verts;

		size_t cacheStart;
		VBO*		vbo;

		bool active;
		bool cacheActive;

	public:
		LineBatch(Renderer* renderer, ResourceManager* resourceManager, const RenderState &baseRenderState = RenderState());
		~LineBatch();
		RenderState renderState;
		class Uniforms
		{
		public:
			enum
			{
				projectionMatrix,
				matrix,
			};
		};
		blib::Shader* shader;
		glm::mat4 matrix;			//TODO: make private
		float thickness;

		virtual void resizeGl(int width, int height, int offsetX, int offsetY);

		virtual void begin(const glm::mat4 &matrix = glm::mat4(), float thickness = 1);
		virtual void end();

		virtual void draw(glm::vec2 v1, glm::vec2 v2, glm::vec4 color = glm::vec4(1,1,0,1), glm::mat4 transform = glm::mat4());
		virtual void draw(const blib::IDrawableLine &drawable, glm::vec4 color = glm::vec4(1,1,0,1), bool drawNormal = false, glm::mat4 transform = glm::mat4());


		virtual void startCache();
		virtual Cache* getCache();
		virtual void drawCache(Cache* cache);

	};

}

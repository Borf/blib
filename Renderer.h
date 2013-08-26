#pragma once

#include <blib/Shader.h>
#include <blib/RenderState.h>
#include <vector>

namespace blib
{

	class Renderer
	{
	protected:
		class RenderBlockBase
		{
		public:
			RenderState renderState;
			Shader::State state;

			virtual void setVertexAttributes() = 0;
			virtual float* firstVertex() = 0;
			virtual int vertexCount() = 0;
		};

		template <class T>
		class RenderBlock : public RenderBlockBase
		{
		public:
			std::vector<T> vertices;
			virtual void setVertexAttributes()
			{
				T::setAttribPointers(&vertices[0]);
			}
			virtual int vertexCount()
			{
				return vertices.size();
			}
		};
		std::vector<RenderBlockBase*> toRender;

	public:
		template<class T>
		void drawTriangles(const RenderState& renderState, std::vector<T> &vertices)
		{
			RenderBlock<T>* block = new RenderBlock<T>();
			block->renderState = renderState;
			block->state = renderState.activeShader->state;
			block->vertices = vertices;
		}
		virtual void flush() = 0;
	};
}
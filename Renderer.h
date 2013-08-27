#pragma once

#include <blib/Shader.h>
#include <blib/RenderState.h>
#include <vector>

namespace blib
{

	class Renderer
	{
	protected:
		class Render
		{
		public:
			enum Command {
				Clear,
				DrawTriangles,
			} command;
			virtual ~Render() {};
			RenderState renderState;
			Shader::State state;

			virtual void setVertexAttributes() = 0;
			virtual int vertexCount() = 0;
		};

		template <class T>
		class RenderBlock : public Render
		{
		public:
			std::vector<T> vertices;
			int count;
			virtual void setVertexAttributes()
			{
				if(renderState.activeVbo != NULL)
					T::setAttribPointers();
				else
					T::setAttribPointers(&vertices[0]);
			}
			virtual int vertexCount()
			{
				if(renderState.activeVbo != NULL)
					return count;
				else
					return vertices.size();
			}
			virtual ~RenderBlock() { vertices.clear(); };
		};


		class RenderClear : public Render
		{
		public:
			glm::vec4 color;
			bool clearColor;
			bool clearDepth;
			bool clearStencil;

			virtual void setVertexAttributes()
			{
			}

			virtual int vertexCount()
			{
				return 0;
			}
		};

		std::vector<Render*> toRender;

	public:
		enum ClearOptions
		{
			Color = 1,
			Depth = 2,
			Stencil = 4,
		};


		template<class T>
		void drawTriangles(const RenderState& renderState, std::vector<T> &vertices)
		{
			RenderBlock<T>* block = new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->renderState = renderState;
			block->state = renderState.activeShader->state;
			block->vertices = vertices;
			toRender.push_back(block);
		}

		template<class T>
		void drawTriangles(const RenderState& renderState, int count)
		{
			RenderBlock<T>* block = new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->count = count;
			block->renderState = renderState;
			block->state = renderState.activeShader->state;
			toRender.push_back(block);
		}

		void clear(const RenderState& renderState, const glm::vec4 &color, int bits)
		{
			RenderClear* block = new RenderClear();
			block->command = Render::Clear;
			block->color = color;
			block->renderState = renderState;
			block->clearColor = (bits & Color) != 0;
			block->clearDepth = (bits & Depth) != 0;
			block->clearStencil = (bits & Stencil) != 0;
			toRender.push_back(block);
		}


		virtual void flush() = 0;
	};
}
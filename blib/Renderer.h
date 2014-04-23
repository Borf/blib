#pragma once

#include <blib/Shader.h>
#include <blib/RenderState.h>
#include <blib/VBO.h>
#include <blib/util/ListAllocator.h>
#include <blib/util/Thread.h>
#include <vector>


namespace blib
{
	class App;

	class Renderer
	{
	public:
		virtual ~Renderer(){};
	protected:
		bool enabledVertexAttributes[10];

		class Render
		{
		public:
			enum Command {
				Clear,
				SetVbo,
				DrawTriangles,
				DrawLines,
				DrawPoints,
				SetShaderState,
				SetSubTexture,
				SetViewPort,
				Unproject,
			} command;
			virtual ~Render() {};
			RenderState renderState;
			char shaderState[1024];

			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex) = 0;
			virtual int vertexCount() = 0;
			virtual void perform(float* firstVertex) {};
		};

		template <class T>
		class RenderBlock : public Render
		{
		public:
			int vertexStart;
			int count;
			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)
			{
				if(renderState.activeVbo != NULL)
					T::setAttribPointers(enabledVertices, 0);
				else
					T::setAttribPointers(enabledVertices, firstVertex + vertexStart);
			}
			virtual int vertexCount()
			{
				return count;
			}
			virtual ~RenderBlock() { };
		};


		class RenderClear : public Render
		{
		public:
			glm::vec4 color;
			bool clearColor;
			bool clearDepth;
			bool clearStencil;

			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)
			{
			}

			virtual int vertexCount()
			{
				return 0;
			}
		};

		template <class T>
		class RenderSetVbo : public Render
		{
		public:
			VBO* vbo;
			int vertexStart;
			int count;

			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)
			{
			}
			virtual int vertexCount()
			{
			//	return vertices.size();
				return 0;
			}
			virtual ~RenderSetVbo() { };
			virtual void perform(float* firstVertex)
			{
				vbo->setData(count, (T*)(firstVertex + vertexStart));
			}
		};

		class RenderSetSubTexture : public Render
		{
		public:
			Texture* texture;
			int x;
			int y;
			int width;
			int height;
			char* data;
			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)
			{
			}

			virtual int vertexCount()
			{
				return 0;
			}
		};
		class RenderSetShaderState : public Render
		{
		public:
			Shader* shader;

			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)			{			}
			virtual int vertexCount()			{ return 0; }
		};

		class RenderSetViewPort : public Render
		{
		public:
			int width;
			int height;
			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)			{			}
			virtual int vertexCount()			{				return 0;			}
		};

		class RenderUnproject : public Render
		{
		public:
			glm::vec2 mousePosition;
			glm::vec4* target;
			glm::mat4 modelMatrix;
			glm::mat4 projectionMatrix;


			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)			{			}
			virtual int vertexCount()			{ return 0; }

		};



		int activeLayer;
		int vertexIndex[2];
		std::vector<Render*> toRender[2];
		float* vertices[2];
		blib::util::ListAllocator allocators[2];

	public:
		enum ClearOptions
		{
			Color = 1,
			Depth = 2,
			Stencil = 4,
		};


		RenderState renderState;

		Renderer()
		{
			activeLayer = 0;
			vertices[0] = new float[1024*1024*50]; // 50M floats
			vertices[1] = new float[1024*1024*50]; // 50M floats
			vertexIndex[0] = 0;
			vertexIndex[1] = 0;
			for(int i = 0; i < 10; i++)
				enabledVertexAttributes[i] = false;

		}

		template<class T>
		void drawTriangles(const std::vector<T> &vertices)
		{
			drawTriangles(vertices, renderState);
		}

		template<class T>
		void drawTriangles(const std::vector<T> &vertices, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = vertexIndex[activeLayer];
			block->count = vertices.size();
			memcpy(this->vertices[activeLayer]+vertexIndex[activeLayer], &vertices[0], sizeof(T) * vertices.size());
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * vertices.size();
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void drawTriangles(int count)
		{
			drawTriangles<T>(count, renderState);
		}

		template<class T>
		void drawTriangles(int count, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->vertexStart = 0;
			block->count = count;
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void drawTriangles(int first, int count, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->vertexStart = first;// * (T::size() / sizeof(float));
			block->count = count;
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			toRender[activeLayer].push_back(block);
		}


		template<class T>
		void drawTriangles(T* first, int count)
		{
			drawTriangles(first, count, renderState);
		}

		template<class T>
		void drawTriangles(T* first, int count, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawTriangles;	//TODO : move to constructor
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = vertexIndex[activeLayer];
			block->count = count;
			memcpy(this->vertices[activeLayer]+vertexIndex[activeLayer], first, sizeof(T) * count);
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * count;
			toRender[activeLayer].push_back(block);
		}
		void clear(const glm::vec4 &color, int bits)
		{
			clear(color, bits, renderState);
		}
		void clear(const glm::vec4 &color, int bits, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderClear* block = allocators[activeLayer].get<RenderClear>(); //new RenderClear();
			block->command = Render::Clear;
			block->color = color;
			block->renderState = renderState;
			block->clearColor = (bits & Color) != 0;
			block->clearDepth = (bits & Depth) != 0;
			block->clearStencil = (bits & Stencil) != 0;
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void setVbo(VBO* vbo, const std::vector<T> &vertices)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderSetVbo<T>* block = allocators[activeLayer].get<RenderSetVbo<T>>(); //new RenderSetVbo<T>();
			block->command = Render::SetVbo;	//TODO : move to constructor
			block->vertexStart = vertexIndex[activeLayer];
			block->count = vertices.size();
			memcpy(this->vertices[activeLayer]+vertexIndex[activeLayer], &vertices[0], sizeof(T) * vertices.size());
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * vertices.size();
			block->vbo = vbo;
			toRender[activeLayer].push_back(block);
		}


		template<class T>
		void drawLines(T* first, int count)
		{
			drawLines(first, count, renderState);
		}

		template<class T>
		void drawLines(T* first, int count, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawLines;	//TODO : move to constructor
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = vertexIndex[activeLayer];
			block->count = count;
			memcpy(this->vertices[activeLayer] + vertexIndex[activeLayer], first, sizeof(T)* count);
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * count;
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void drawLines(int count, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawLines;	//TODO : move to constructor
			block->vertexStart = 0;
			block->count = count;
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			toRender[activeLayer].push_back(block);
		}



		template<class T>
		void drawPoints(const std::vector<T> &vertices, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
			block->command = Render::DrawPoints;	//TODO : move to constructor
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = vertexIndex[activeLayer];
			block->count = vertices.size();
			memcpy(this->vertices[activeLayer]+vertexIndex[activeLayer], &vertices[0], sizeof(T) * vertices.size());
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * vertices.size();
			toRender[activeLayer].push_back(block);
		}

		void setTextureSubImage(blib::Texture* texture, int x, int y, int width, int height, char* data)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderSetSubTexture* command = allocators[activeLayer].get<RenderSetSubTexture>(); //new RenderSetSubTexture();
			command->command = Render::SetSubTexture;
			command->texture = texture;
			command->x = x;
			command->y = y;
			command->width = width;
			command->height = height;
			command->data = new char[width*height*4];
			memcpy(command->data, data, width*height*4);
			toRender[activeLayer].push_back(command);
		}

/*		void setShaderState(Shader* shader)
		{
			RenderSetShaderState* command = allocators[activeLayer].get<RenderSetShaderState>(); //new RenderSetSubTexture();
			command->command = Render::SetShaderState;
			command->shader = shader;
		//	command->shaderState = shader->state;
			toRender[activeLayer].push_back(command);
		}*/
		
		void setViewPort(int width, int height)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderSetViewPort* command = allocators[activeLayer].get<RenderSetViewPort>();
			command->command = Render::SetViewPort;
			command->width = width;
			command->height = height;
			toRender[activeLayer].push_back(command);
		}

		void unproject(glm::vec2 mousePosition, glm::vec4* target, const glm::mat4 &modelMatrix, const glm::mat4 &projectionMatrix)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
			RenderUnproject* command = allocators[activeLayer].get<RenderUnproject>();
			command->command = Render::Unproject;
			command->mousePosition = mousePosition;
			command->target = target;
			command->modelMatrix = modelMatrix;
			command->projectionMatrix = projectionMatrix;

			toRender[activeLayer].push_back(command);
		}



		virtual void flush() = 0;
		App* app;
		void swap()
		{
			activeLayer = 1 - activeLayer;
			vertexIndex[activeLayer] = 0;
//			memset(vertices[activeLayer], 0, 1024 * 1024 * 50 * sizeof(float));
		}



	};
}
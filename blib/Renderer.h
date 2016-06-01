#pragma once

#include <blib/Shader.h>
#include <blib/RenderState.h>
#include <blib/VBO.h>
#include <blib/VIO.h>
#include <blib/util/Log.h>
#include <blib/Util.h>
#include <blib/util/ListAllocator.h>
#include <blib/util/Thread.h>
#include <blib/gl/VBO.h> // uhoh
#include <vector>

#include <blib/config.h>
#if defined(BLIB_IOS)

#else
//#ifdef _DEBUG
#define CUSTOMMEMALLOCATOR
//#endif
#endif

namespace blib
{
	class App;

	namespace math { class Ray; };


	class Renderer
	{
	public:
		virtual ~Renderer()
		{
			delete[] vertices[0];
			delete[] vertices[1];
		};
	protected:
		bool enabledVertexAttributes[10];

		class Render
		{
		public:
			enum Command {
				Clear,
				SetVbo,
				SetVio,
				DrawTriangles,
				DrawLines,
				DrawPoints,
				DrawIndexedTriangles,
				SetShaderState,
				SetSubTexture,
				SetViewPort,
				Unproject,
				SaveFbo,
				SetVboSub,
			} command;
			Render()
			{
//				blib::util::callstack();
			}
			virtual ~Render() {};
			RenderState renderState;
			char shaderState[1024*8];

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
			float lineThickness;
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
			bool empty;

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
				if(empty)
					vbo->setData(count, NULL);
				else
					vbo->setData(count, (T*)(firstVertex + vertexStart));
			}
		};

		template <class T>
		class RenderSetVboSub : public Render
		{
		public:
			VBO* vbo;
			int vertexStart;
			int count;
			int position;

			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)
			{
			}
			virtual int vertexCount()
			{
				//	return vertices.size();
				return 0;
			}
			virtual ~RenderSetVboSub() { };
			virtual void perform(float* firstVertex)
			{
				vbo->setSubData(position, count, (T*)(firstVertex + vertexStart));
			}
		};
		template <class T>
		class RenderSetVio : public Render
		{
		public:
			VIO* vio;
			std::vector<T> indices;

			
			virtual ~RenderSetVio() { };
			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex){}
			virtual int vertexCount(){ return 0; }
			virtual void perform(float* firstVertex)
			{
				if (!indices.empty())
 					vio->setData(indices.size(), &indices[0]);
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
			int left;
			int top;
			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)			{			}
			virtual int vertexCount()			{				return 0;			}
		};

		class RenderUnproject : public Render
		{
		public:
			glm::vec2 mousePosition;
			glm::vec4* target;

			blib::math::Ray* ray;

			glm::mat4 modelMatrix;
			glm::mat4 projectionMatrix;


			virtual void setVertexAttributes(bool enabledVertices[10], float* firstVertex)			{			}
			virtual int vertexCount()			{ return 0; }

		};

		class RenderSaveFbo : public Render
		{
		public:
			FBO* fbo;
			std::string filename;

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
			vertices[0] = new float[1024*1024*15]; // 50M floats
			vertices[1] = new float[1024*1024*15]; // 50M floats
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
			if (vertices.empty())
				return;
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
		void drawIndexedTriangles(int begin, int count, const RenderState& renderState)
		{
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
			block->command = Render::DrawIndexedTriangles;	//TODO : move to constructor
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = begin;
			block->count = count;
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
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderClear* block = allocators[activeLayer].get<RenderClear>(); //new RenderClear<T>();
#else
			RenderClear* block = new RenderClear();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderSetVbo<T>* block = allocators[activeLayer].get<RenderSetVbo<T>>(); //new RenderSetVbo<T>();
#else
			RenderSetVbo<T>* block = new RenderSetVbo<T>();
#endif
			block->command = Render::SetVbo;	//TODO : move to constructor
			block->vertexStart = vertexIndex[activeLayer];
			block->count = vertices.size();
			if (vertices.size() > 0)
				memcpy(this->vertices[activeLayer]+vertexIndex[activeLayer], &vertices[0], sizeof(T) * vertices.size());
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * vertices.size();
			block->vbo = vbo;
			((blib::gl::VBO*)vbo)->length = vertices.size();
			toRender[activeLayer].push_back(block);
		}



		template<class T>
		void setVbo(VBO* vbo, T* vertices, int count)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderSetVbo<T>* block = allocators[activeLayer].get<RenderSetVbo<T>>(); //new RenderSetVbo<T>();
#else
			RenderSetVbo<T>* block = new RenderSetVbo<T>();
#endif
			block->command = Render::SetVbo;	//TODO : move to constructor
			block->vertexStart = vertexIndex[activeLayer];
			block->count = count;
			if (count > 0 && vertices != NULL)
			{
				memcpy(this->vertices[activeLayer] + vertexIndex[activeLayer], vertices, sizeof(T) * count);
				vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * count;
			}
			
			block->empty = vertices == NULL;
			block->vbo = vbo;
		//	((blib::gl::VBO*)vbo)->length = count;
			toRender[activeLayer].push_back(block);
		}


		template<class T>
		void setVboSub(VBO* vbo, int position, T* vertices, int count)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderSetVboSub<T>* block = allocators[activeLayer].get<RenderSetVboSub<T>>(); //new RenderSetVbo<T>();
#else
			RenderSetVboSub<T>* block = new RenderSetVboSub<T>();
#endif
			block->command = Render::SetVboSub;	//TODO : move to constructor
			block->position = position;
			block->vertexStart = vertexIndex[activeLayer];
			block->count = count;
			if (count > 0)
				memcpy(this->vertices[activeLayer] + vertexIndex[activeLayer], vertices, sizeof(T) * count);
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * count;
			block->vbo = vbo;
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void setVio(VIO* vio, const std::vector<T> &indices)
		{
#ifdef CUSTOMMEMALLOCATOR
			RenderSetVio<T>* block = allocators[activeLayer].get<RenderSetVio<T>>();
#else
			RenderSetVio<T>* block = new RenderSetVio<T>();
#endif
			block->command = Render::SetVio;
			block->indices = indices;
			block->vio = vio;
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
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
			block->command = Render::DrawLines;	//TODO : move to constructor
			block->vertexStart = 0;
			block->count = count;
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void drawLines(int count, float thickness, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
			block->lineThickness = thickness;
			block->command = Render::DrawLines;	//TODO : move to constructor
			block->vertexStart = 0;
			block->count = count;
			block->renderState = renderState;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		inline void drawLines(const std::vector<T> &vertices, float thickness = 1)
		{
			drawLines(vertices, thickness, renderState);
		}
		template<class T>
		inline void drawLines(const std::vector<T> &vertices, const RenderState& renderState)
		{
			drawLines(vertices, 1, renderState);
		}

		template<class T>
		void drawLines(const std::vector<T> &vertices, float thickness, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
			block->command = Render::DrawLines;	//TODO : move to constructor
			block->renderState = renderState;
			block->lineThickness = thickness;
			memcpy(block->shaderState, renderState.activeShader->uniformData, renderState.activeShader->uniformSize);
			block->vertexStart = vertexIndex[activeLayer];
			block->count = vertices.size();
			memcpy(this->vertices[activeLayer] + vertexIndex[activeLayer], &vertices[0], sizeof(T)* vertices.size());
			vertexIndex[activeLayer] += (sizeof(T) / sizeof(float)) * vertices.size();
			toRender[activeLayer].push_back(block);
		}

		template<class T>
		void drawPoints(const std::vector<T> &vertices, const RenderState& renderState)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderBlock<T>* block = allocators[activeLayer].get<RenderBlock<T>>(); //new RenderBlock<T>();
#else
			RenderBlock<T>* block = new RenderBlock<T>();
#endif
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
#ifdef CUSTOMMEMALLOCATOR
			RenderSetSubTexture* command = allocators[activeLayer].get<RenderSetSubTexture>(); //new RenderSetSubTexture();
#else
			RenderSetSubTexture* command = new RenderSetSubTexture();
#endif
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
		
		void setViewPort(int left, int top, int width, int height)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderSetViewPort* command = allocators[activeLayer].get<RenderSetViewPort>();
#else
			RenderSetViewPort* command = new RenderSetViewPort();
#endif
			command->command = Render::SetViewPort;
			command->left = left;
			command->top = top;
			command->width = width;
			command->height = height;
			toRender[activeLayer].push_back(command);
		}

		void unproject(glm::vec2 mousePosition, glm::vec4* target, blib::math::Ray* ray, const glm::mat4 &modelMatrix, const glm::mat4 &projectionMatrix)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderUnproject* command = allocators[activeLayer].get<RenderUnproject>();
#else
			RenderUnproject* command = new RenderUnproject();
#endif
			command->command = Render::Unproject;
			command->mousePosition = mousePosition;
			command->target = target;
			command->ray = ray;
			command->modelMatrix = modelMatrix;
			command->projectionMatrix = projectionMatrix;

			toRender[activeLayer].push_back(command);
		}


		void saveFbo(FBO* fbo, std::string filename)
		{
			//assert(blib::util::Thread::getCurrentThreadName() == "UpdateThread");
#ifdef CUSTOMMEMALLOCATOR
			RenderSaveFbo* command = allocators[activeLayer].get<RenderSaveFbo>();
#else
			RenderSaveFbo* command = new RenderSaveFbo();
#endif
			command->command = Render::SaveFbo;
			command->fbo = fbo;
			command->filename = filename;
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


		void clearCommands()
		{
#ifdef CUSTOMMEMALLOCATOR
			allocators[1 - activeLayer].clear(); // deletes all objects
#else
			for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
				delete toRender[1-activeLayer][i];
#endif

			toRender[1 - activeLayer].clear();
		}



	};
}
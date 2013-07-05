#ifndef __VBO_H__
#define __VBO_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/util/NotCopyable.h>
#include <blib/gl/GlInitRegister.h>

namespace blib
{
	namespace gl
	{

		template <class T>
		class VBO : public blib::util::NotCopyable, public GlInitRegister
		{
		private:
			GLuint vbo;

			T* element;
			int length;
		public:
			VBO()
			{
				length = 0;
				element = NULL;
				vbo = -1;
			}
			~VBO()
			{
				if(vbo != -1)
					glDeleteBuffers(1, &vbo);
			}
			void initGl()
			{
				if(vbo == -1)
					glGenBuffers(1, &vbo);
			}

			void setData(int length, T* data, GLenum usage)
			{
				this->length = length;
				bind();
				int size = T::size() * length;
				glBufferData(GL_ARRAY_BUFFER, T::size() * length, data, usage);
			}

			void setSubData(int offset, int length, T* data)
			{
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, T::size() * offset, T::size() * length, data);
			}


			void bind()
			{
				if(vbo == -1)
					initGl();
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
			}

			void unBind()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			int elementSize()
			{
				return T::getSize();
			}

			int getLength()
			{
				return length;
			}
#ifndef ANDROID
			T* mapData(GLenum access)
			{
				bind();
				element = (T*)glMapBuffer(GL_ARRAY_BUFFER, access);
				return element;
			}
			void unmapData()
			{
				bind();
				glUnmapBuffer(GL_ARRAY_BUFFER);
				element = NULL;
			}

			T& operator [](int index)
			{
				if(element == NULL)
					throw "Use mapData before accessing";
				return element[index];
			}
#endif



			void setVAO()
			{
				T::setVAO(T::getSize());
			}

		};
	}

}


#endif

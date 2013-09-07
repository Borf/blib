#ifndef __VIO_H__
#define __VIO_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/util/NotCopyable.h>
#include <blib/gl/GlInitRegister.h>
#include <blib/VIO.h>

namespace blib
{
	namespace gl
	{

		template <class T>
		class VIO : public blib::VIO, public blib::util::NotCopyable, public GlInitRegister
		{
		private:
			GLuint vio;

			T* element;
			int length;
		public:
			VIO()
			{
				length = 0;
				element = NULL;
				vio = -1;
			}
			~VIO()
			{
				if(vio != -1)
					glDeleteBuffers(1, &vio);
			}

			void initGl()
			{
				if(vio == -1)
					glGenBuffers(1, &vio);
			}

			void setData(int length, T* data, GLenum usage)
			{
				this->length = length;
				bind();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * length, data, usage);
			}


			void bind()
			{
				if(vio == -1)
					initGl();
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
			}

			void unBind()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

			int elementSize()
			{
				return T::getSize();
			}

			int getLength()
			{
				return length;
			}

			T* mapData(GLenum access)
			{
				bind();
				element = (T*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, access);
				return element;
			}
			void unmapData()
			{
				bind();
				glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
				element = NULL;
			}

			T& operator [](int index)
			{
				if(element == NULL)
					throw "Use mapData before accessing";
				return element[index];
			}
		};
	}
}

#endif
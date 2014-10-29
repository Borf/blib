#ifndef __VIO_H__
#define __VIO_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/util/NotCopyable.h>
#include <blib/VIO.h>

namespace blib
{
	namespace gl
	{
		class VIO : public blib::VIO, public blib::util::NotCopyable
		{
		private:
			GLuint vio;
			int length;
		public:
			VIO()
			{
				length = 0;
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

			void setData(int length, void* data)
			{
				this->length = length;
				bind();
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * length, data, GL_STATIC_DRAW);
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

			int getLength()
			{
				return length;
			}

		};
	}
}

#endif
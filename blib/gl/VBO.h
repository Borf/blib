#ifndef __VBO_H__
#define __VBO_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/VBO.h>
#include <blib/util/NotCopyable.h>
#include <blib/util/FastDelegate.h>
#include <blib/gl/GlInitRegister.h>

namespace blib
{
	namespace gl
	{

		class VBO : public blib::VBO, public blib::util::NotCopyable, public GlInitRegister
		{
		private:
			GLuint vbo;
			int length;
		public:
			VBO()
			{
				length = 0;
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

			void setData(int length, void* data)
			{
				this->length = length;
				bind();
				int size = elementSize() * length;
				glBufferData(GL_ARRAY_BUFFER, elementSize() * length, data, GL_STATIC_DRAW);
			}

			void setSubData(int offset, int length, void* data)
			{
				bind();
				glBufferSubData(GL_ARRAY_BUFFER, elementSize() * offset, elementSize() * length, data);
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

			int getLength()
			{
				return length;
			}

		};
	}

}


#endif

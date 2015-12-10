#ifndef __VBO_H__
#define __VBO_H__

#include <blib/config.h>

#if defined(BLIB_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(BLIB_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#endif

#include <blib/VBO.h>
#include <blib/util/NotCopyable.h>

namespace blib
{
	namespace gl
	{

		class VBO : public blib::VBO, public blib::util::NotCopyable
		{
		private:
			GLuint vbo;
		public:
			int length;

			VBO()
			{
				name = "VBO";
				length = 0;
				vbo = 0;
			}
			~VBO()
			{
				if(vbo != 0)
					glDeleteBuffers(1, &vbo);
				vbo = 0;
			}
			void initGl()
			{
				if(vbo == 0)
					glGenBuffers(1, &vbo);
			}

			void setData(int len, void* data)
			{
				assert(elementSize);
				this->length = len;
				bind();
				assert(vbo != 0);
				glBufferData(GL_ARRAY_BUFFER, elementSize() * length, data, GL_DYNAMIC_DRAW);
			}

			void setSubData(int offset, int len, void* data)
			{
				bind();
				assert(vbo != 0);
				assert(length != 0);
				glBufferSubData(GL_ARRAY_BUFFER, elementSize() * offset, elementSize() * len, data);
			}


			void bind()
			{
				if(vbo == 0)
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

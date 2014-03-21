#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/FBO.h>

namespace blib
{
	namespace gl
	{
		class FBO : public blib::FBO
		{

			GLuint texid;
			GLuint fbo;
			GLuint depthBuffer;
			GLuint stencilBuffer;
		public:
			bool depth;
			bool stencil;


			FBO();
			~FBO();

			virtual void setSize(int width, int height);

			void bind();
			void unbind();

			int getHeight();
			int getWidth();

			void use();
		private:
			void init();
		};

	}
}

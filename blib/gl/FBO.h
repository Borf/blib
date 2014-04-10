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

			GLuint texids[3];
			GLuint fbo;
			GLuint depthBuffer;
			GLuint stencilBuffer;
		public:


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

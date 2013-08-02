#include "FBO.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace blib
{
	namespace gl
	{
		FBO::FBO(int width, int height, bool depth, bool stencil)
		{
			depthBuffer = 0;
			stencilBuffer = 0;
			texid = 0;
			fbo = 0;
			this->width = width;
			this->height = height;
			this->originalHeight = height;
			this->originalWidth = width;
			this->depth = depth;
			this->stencil = stencil;
		}
		FBO::~FBO()
		{

		}


		void blib::gl::FBO::initGl()
		{
			glGenTextures(1, &texid);
			glBindTexture(GL_TEXTURE_2D, texid);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	

			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);

			if(depth || stencil)
			{
				glGenRenderbuffers(1, &depthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		#ifdef ANDROID
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		#else
				if(depth && !stencil)
					glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
				else if(!depth && stencil)
					glRenderbufferStorage( GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
				else if (depth && stencil)
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
		#endif
				if(depth)
					glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
				if(stencil)
					glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
			}

			unbind();
		}

		void FBO::bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			if(depthBuffer > 0)
				glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		}

		void FBO::unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			if(depthBuffer > 0)
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

		int FBO::getWidth()
		{
			return width;
		}

		int FBO::getHeight()
		{
			return height;
		}
	}
}

#include "FBO.h"

#include <blib/config.h>

#ifdef WIN32
#include <windows.h>
#endif

namespace blib
{
	namespace gl
	{
		FBO::FBO()
		{
			depthBuffer = 0;
			stencilBuffer = 0;
			textureCount = 1;
			texids[0] = 0;
			texids[1] = 0;
			texids[2] = 0;
			fbo = 0;
			this->width = 0;
			this->height = 0;
			this->originalHeight = 0;
			this->originalWidth = 0;
			this->depth = false;
			this->stencil = false;
		}
		FBO::~FBO()
		{

		}

		void FBO::setSize(int width, int height)
		{
			this->width = width;
			this->height = height;
			this->originalWidth = width;
			this->originalHeight = height;
			fbo = 0;
			//TODO: DELETE FBO!
		}


		void FBO::init()
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			//http://gamedev.stackexchange.com/questions/59194/multiple-render-targets-multiple-fragment-shaders
			for (int i = 0; i < textureCount; i++)
			{
				glGenTextures(1, &texids[i]);
				glBindTexture(GL_TEXTURE_2D, texids[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texids[i], 0);
			}

			if(depth || stencil)
			{
				glGenRenderbuffers(1, &depthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
#if defined(BLIB_ANDROID) || defined(BLIB_IOS)
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

		void FBO::bind() //todo: keep a stack of FBOs
		{
			if(fbo == 0)
			{
				init();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			if(depthBuffer > 0)
				glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
#if !defined(BLIB_ANDROID) && !defined(BLIB_IOS)
			GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, buffers);
#endif


		}

		void FBO::unbind() //todo: pop the FBOs
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

		void FBO::use()
		{
			if(texids[0] == 0)
				init();
			glBindTexture(GL_TEXTURE_2D, texids[0]);
			if (textureCount > 1)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texids[1]);
				glActiveTexture(GL_TEXTURE0);
			}
		}
	}
}

#pragma once

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <gl/glew.h>
#endif

class FBO
{
	int width;
	int height;
	GLuint fbo;
	GLuint depthBuffer;
public:
	FBO(int width, int height, bool depth = false);
	~FBO();

	void bind();
	void unbind();
	GLuint texid;

	int getHeight();
	int getWidth();

};

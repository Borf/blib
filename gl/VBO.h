#ifndef __VBO_H__
#define __VBO_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

template <class T>
class VBO
{
private:
	GLuint vbo;

	T* element;
	int length;
	VBO(const VBO &other)
	{
		throw "do not copy!";
	}

public:
	VBO()
	{
		length = 0;
		element = NULL;
		glGenBuffers(1, &vbo);
	}
	~VBO()
	{
		glDeleteBuffers(1, &vbo);
	}

	void setData(int length, T* data, GLenum usage)
	{
		this->length = length;
		bind();
		glBufferData(GL_ARRAY_BUFFER, T::getSize() * length, data, usage);
	}


	void bind()
	{
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

/*	T* mapData(GLenum access)
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
	}*/



	void setVAO()
	{
		T::setVAO(T::getSize());
	}

};


#endif

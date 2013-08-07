#ifndef __VERTEX_H__
#define __VERTEX_H__

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>

class Vertex
{
protected:
public:
	static void setAttribPointers(void* offset = NULL, int *index = NULL, int totalSize = size())
	{
	}
	static const int size() { return 0; };
};


#define VertexDefBegin(className, memberName, memberType, count, base)	\
	class className : public base \
	{ \
	public: \
	\
		className() {} \
		static void setAttribPointers(void* offset = NULL, int *index = NULL, int totalSize = size()) \
		{\
			int tmpIndex = 0;\
			if(!index)\
				index = &tmpIndex;\
			base::setAttribPointers(offset, index, totalSize);\
			glVertexAttribPointer((*index)++, count, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset + base::size()));\
		}\
		static int size() { return base::size() + count*sizeof(GL_FLOAT); }\
		\
		memberType memberName; \
		\

#define VertexDefEnd() }





VertexDefBegin(VertexP3,								position,	glm::vec3, 3, Vertex)
	VertexP3(glm::vec3 position) : position(position) {};
VertexDefEnd();

VertexDefBegin(VertexP2,								position,	glm::vec2, 2, Vertex)
	VertexP2(glm::vec2 position) : position(position) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2,								texCoord,	glm::vec2, 2, VertexP3)
	VertexP3T2(glm::vec3 position, glm::vec2 texCoord) : VertexP3(position), texCoord(texCoord) {};
VertexDefEnd();

VertexDefBegin(VertexP2T2,								texCoord,	glm::vec2, 2, VertexP2)
	VertexP2T2(glm::vec2 position, glm::vec2 texCoord) : VertexP2(position), texCoord(texCoord) {};
VertexDefEnd();

VertexDefBegin(VertexP2T2T2,							texCoord2,	glm::vec2, 2, VertexP2T2)
	VertexP2T2T2(glm::vec2 position, glm::vec2 texCoord, glm::vec2 texCoord2) : VertexP2T2(position, texCoord), texCoord2(texCoord2) {};
VertexDefEnd();

VertexDefBegin(VertexP2T3,								texCoord,	glm::vec3, 3, VertexP2)
	VertexP2T3(glm::vec2 position, glm::vec3 texCoord) : VertexP2(position), texCoord(texCoord) {};
VertexDefEnd();

VertexDefBegin(VertexP2T3T2,							texCoord2,	glm::vec2, 2, VertexP2T3)
	VertexP2T3T2(glm::vec2 position, glm::vec3 texCoord, glm::vec2 texCoord2) : VertexP2T3(position, texCoord), texCoord2(texCoord2) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2C4,							color,		glm::vec4, 4, VertexP3T2)
	VertexP3T2C4(glm::vec3 position, glm::vec2 texCoord, glm::vec4 color) : VertexP3T2(position, texCoord), color(color) {};
VertexDefEnd();

VertexDefBegin(VertexP2C4,								color,		glm::vec4, 4, VertexP2)
	VertexP2C4(glm::vec2 position, glm::vec4 color) : VertexP2(position), color(color) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2C3,							color,		glm::vec3, 3, VertexP3T2)
	VertexP3T2C3(glm::vec3 position, glm::vec2 texCoord, glm::vec3 color) : VertexP3T2(position, texCoord), color(color) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2C1,							filler,			int, 1, VertexP3T2)
	VertexP3T2C1(glm::vec3 position, glm::vec2 texCoord, glm::vec4 color) : VertexP3T2(position, texCoord) 
	{
		//int r = color.
	};
VertexDefEnd();

VertexDefBegin(VertexP2TC4,								color,		glm::vec4, 4, VertexP2T2)
	VertexP2TC4(glm::vec2 position, glm::vec2 texCoord, glm::vec4 color) : VertexP2T2(position, texCoord), color(color) {};
VertexDefEnd();

#endif

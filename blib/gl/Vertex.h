#ifndef __VERTEX_H__
#define __VERTEX_H__


#include <glm/glm.hpp>

namespace blib
{
	class Vertex
	{
	protected:
	public:
		static void setAttribPointers(bool enabledVertexAttributes[10], void* offset = NULL, int *index = NULL, int totalSize = size())
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
		static void setAttribPointers(bool enabledVertexAttributes[10], void* offset = NULL, int *index = NULL, int totalSize = size());\
		static int size();\
		\
		memberType memberName; \
		\

#define VertexDefEnd() }


#define VertexDef(className, memberName, memberType, count, base)	\
	void className::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
{\
	int tmpIndex = 0;\
	if(!index)\
	index = &tmpIndex;\
	base::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);\
	if(!enabledVertexAttributes[*index])\
	glEnableVertexAttribArray(*index);\
	enabledVertexAttributes[*index] = true;\
	glVertexAttribPointer((*index)++, count, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset + base::size()));\
	if(index == &tmpIndex)\
{\
	for(int i = *index; i < 10; i++)\
	if(enabledVertexAttributes[i])\
{\
	enabledVertexAttributes[i] = false;\
	glDisableVertexAttribArray(i);\
}\
}\
}\
	int className::size() { return base::size() + count*sizeof(GL_FLOAT); }\



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

VertexDefBegin(VertexP2C4F1,							rotation,		float, 1, VertexP2C4)
	VertexP2C4F1(glm::vec2 position, glm::vec4 color, float rotation) : VertexP2C4(position, color), rotation(rotation) {};
VertexDefEnd();

VertexDefBegin(VertexP2C4F1F1,							_size,		float, 1, VertexP2C4F1)
	VertexP2C4F1F1(glm::vec2 position, glm::vec4 color, float rotation, float _size) : VertexP2C4F1(position, color, rotation), _size(_size) {};
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

VertexDefBegin(VertexP2T2C4,								color,		glm::vec4, 4, VertexP2T2)
	VertexP2T2C4(glm::vec2 position, glm::vec2 texCoord, glm::vec4 color) : VertexP2T2(position, texCoord), color(color) {};
VertexDefEnd();


}
#endif

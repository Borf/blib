#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <blib/config.h>
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
	int tmpIndex = 0; \
	if(!index)\
	index = &tmpIndex;\
	base::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);\
	if(!enabledVertexAttributes[*index]/* && glEnableVertexAttribArray*/)\
	glEnableVertexAttribArray(*index);\
	enabledVertexAttributes[*index] = true;\
/*	if(glVertexAttribPointer)*/\
	glVertexAttribPointer((*index)++, count, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset + base::size()));\
	if(index == &tmpIndex)\
{\
	for(int i = *index; i < 10; i++)\
	if(enabledVertexAttributes[i])\
{\
	enabledVertexAttributes[i] = false;\
/*	if(glDisableVertexAttribArray)*/\
	glDisableVertexAttribArray(i);\
}\
}\
}\
	int className::size() { return base::size() + count*sizeof(GL_FLOAT); }\

#ifdef BLIB_ANDROID
#define VertexDefi(className, memberName, memberType, count, base)	\
	void className::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
	{\
\
	} \
	int className::size() { return base::size() + count*sizeof(GL_INT); }

#else
#define VertexDefi(className, memberName, memberType, count, base)	\
	void className::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
{\
	int tmpIndex = 0; \
	if(!index)\
	index = &tmpIndex;\
	base::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);\
	if(!enabledVertexAttributes[*index]/* && glEnableVertexAttribArray*/)\
	glEnableVertexAttribArray(*index);\
	enabledVertexAttributes[*index] = true;\
/*	if(glVertexAttribPointer)*/\
	glVertexAttribIPointer((*index)++, count, GL_INT, totalSize, (void*)((char*)offset + base::size()));\
	if(index == &tmpIndex)\
{\
	for(int i = *index; i < 10; i++)\
	if(enabledVertexAttributes[i])\
{\
	enabledVertexAttributes[i] = false;\
/*	if(glDisableVertexAttribArray)*/\
	glDisableVertexAttribArray(i);\
}\
}\
}\
	int className::size() { return base::size() + count*sizeof(GL_INT); }\


#endif



VertexDefBegin(VertexP3,								position,	glm::vec3, 3, Vertex)
	VertexP3(glm::vec3 position) : position(position) {};
VertexDefEnd();

VertexDefBegin(VertexP2,								position,	glm::vec2, 2, Vertex)
	VertexP2(glm::vec2 position) : position(position) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2, texCoord, glm::vec2, 2, VertexP3)
VertexP3T2(glm::vec3 position, glm::vec2 texCoord) : VertexP3(position), texCoord(texCoord) {};
VertexDefEnd();

VertexDefBegin(VertexP3N3, normal, glm::vec3, 3, VertexP3)
VertexP3N3(glm::vec3 position, glm::vec3 normal) : VertexP3(position), normal(normal) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2N3, normal, glm::vec3, 3, VertexP3T2)
VertexP3T2N3(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal) : VertexP3T2(position, texCoord), normal(normal) {};
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

VertexDefBegin(VertexP3C4, color, glm::vec4, 4, VertexP3)
VertexP3C4(glm::vec3 position, glm::vec4 color) : VertexP3(position), color(color) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2C4,							color,		glm::vec4, 4, VertexP3T2)
	VertexP3T2C4(glm::vec3 position, glm::vec2 texCoord, glm::vec4 color) : VertexP3T2(position, texCoord), color(color) {};
VertexDefEnd();

VertexDefBegin(VertexP2C4,								color,		glm::vec4, 4, VertexP2)
	VertexP2C4(glm::vec2 position, glm::vec4 color) : VertexP2(position), color(color) {};
VertexDefEnd();

VertexDefBegin(VertexP2C4T2,							tex1,		glm::vec2, 2, VertexP2C4)
	VertexP2C4T2(glm::vec2 position, glm::vec4 color, glm::vec2 tex1) : VertexP2C4(position, color), tex1(tex1) {};
VertexDefEnd();

VertexDefBegin(VertexP2C4T2T2,							tex2,		glm::vec2, 2, VertexP2C4T2)
	VertexP2C4T2T2(glm::vec2 position, glm::vec4 color, glm::vec2 tex1, glm::vec2 tex2) : VertexP2C4T2(position, color, tex1), tex2(tex2) {};
VertexDefEnd();


VertexDefBegin(VertexP2C4T2T2F1,							_size,		float, 1, VertexP2C4T2T2)
	VertexP2C4T2T2F1(glm::vec2 position, glm::vec4 color, glm::vec2 tex1, glm::vec2 tex2, float _size) : VertexP2C4T2T2(position, color, tex1, tex2), _size(_size) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2T2, texCoord2, glm::vec2, 2, VertexP3T2)
VertexP3T2T2(glm::vec3 position, glm::vec2 texCoord, glm::vec2 texCoord2) : VertexP3T2(position, texCoord), texCoord2(texCoord2) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2T2T2, texCoord3, glm::vec2, 2, VertexP3T2T2)
VertexP3T2T2T2(glm::vec3 position, glm::vec2 texCoord, glm::vec2 texCoord2, glm::vec2 texCoord3) : VertexP3T2T2(position, texCoord, texCoord2), texCoord3(texCoord3) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2T2T2N3, normal, glm::vec3, 3, VertexP3T2T2T2)
VertexP3T2T2T2N3(glm::vec3 position, glm::vec2 texCoord, glm::vec2 texCoord2, glm::vec2 texCoord3, glm::vec3 normal) : VertexP3T2T2T2(position, texCoord, texCoord2, texCoord3), normal(normal) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2T2C3, color, glm::vec3, 3, VertexP3T2T2)
VertexP3T2T2C3(glm::vec3 position, glm::vec2 texCoord, glm::vec2 texCoord2, glm::vec3 color) : VertexP3T2T2(position, texCoord, texCoord2), color(color) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2T2C3N3, normal, glm::vec3, 3, VertexP3T2T2C3)
VertexP3T2T2C3N3(glm::vec3 position, glm::vec2 texCoord, glm::vec2 texCoord2, glm::vec3 color, glm::vec3 normal) : VertexP3T2T2C3(position, texCoord, texCoord2, color), normal(normal){};
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

VertexDefBegin(VertexP2T2C4C4, colorOverlay, glm::vec4, 4, VertexP2T2C4)
VertexP2T2C4C4(glm::vec2 position, glm::vec2 texCoord, glm::vec4 color, glm::vec4 colorOverlay) : VertexP2T2C4(position, texCoord, color), colorOverlay(colorOverlay) {};
VertexDefEnd();


VertexDefBegin(VertexP3T2N3B4, boneIds, glm::ivec4, 4, VertexP3T2N3)
	VertexP3T2N3B4(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal, glm::ivec4 boneIds) : VertexP3T2N3(position, texCoord, normal), boneIds(boneIds) {};
VertexDefEnd();

VertexDefBegin(VertexP3T2N3B4B4, boneWeights, glm::vec4, 4, VertexP3T2N3B4)
	VertexP3T2N3B4B4(glm::vec3 position, glm::vec2 texCoord, glm::vec3 normal, glm::ivec4 boneIds, glm::vec4 boneWeights) : VertexP3T2N3B4(position, texCoord, normal, boneIds), boneWeights(boneWeights) {};
VertexDefEnd();


VertexDefBegin(VertexP3N3C4, color, glm::vec4, 4, VertexP3N3)
VertexP3N3C4(glm::vec3 position, glm::vec3 normal, glm::vec4 color) : VertexP3N3(position, normal), color(color) {};
VertexDefEnd();




VertexDefBegin(VertexP2T2C4P2, position2, glm::vec2, 2, VertexP2T2C4)
VertexP2T2C4P2(glm::vec2 position, glm::vec2 texture, glm::vec4 color, glm::vec2 position2) : VertexP2T2C4(position, texture, color), position2(position2) {};
VertexDefEnd();

VertexDefBegin(VertexP2T2C4P2R1, rotation, float, 1, VertexP2T2C4P2)
VertexP2T2C4P2R1(glm::vec2 position, glm::vec2 texture, glm::vec4 color, glm::vec2 position2, float rotation) : VertexP2T2C4P2(position, texture, color, position2), rotation(rotation) {};
VertexDefEnd();

VertexDefBegin(VertexP2T2C4P2R1S1, scale, float, 1, VertexP2T2C4P2R1)
VertexP2T2C4P2R1S1(glm::vec2 position, glm::vec2 texture, glm::vec4 color, glm::vec2 position2, float rotation, float scale) : VertexP2T2C4P2R1(position, texture, color, position2, rotation), scale(scale) {};
VertexDefEnd();




}
#endif

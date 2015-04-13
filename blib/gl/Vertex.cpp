#include "Vertex.h"

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


//TODO: const reference for constructors
namespace blib
{

VertexDef(VertexP3,								position,	glm::vec3, 3, Vertex)
VertexDef(VertexP2,								position,	glm::vec2, 2, Vertex)
VertexDef(VertexP3T2,							texCoord,	glm::vec2, 2, VertexP3)
VertexDef(VertexP3N3,							normal,		glm::vec3, 3, VertexP3)
VertexDef(VertexP3T2N3,							normal,		glm::vec3, 3, VertexP3T2)
VertexDef(VertexP2T2,							texCoord,	glm::vec2, 2, VertexP2)
VertexDef(VertexP2T2T2,							texCoord2,	glm::vec2, 2, VertexP2T2)
VertexDef(VertexP2T3,							texCoord,	glm::vec3, 3, VertexP2)
VertexDef(VertexP2T3T2,							texCoord2,	glm::vec2, 2, VertexP2T3)
VertexDef(VertexP3T2T2,							texCoord2,	glm::vec2, 2, VertexP3T2)
VertexDef(VertexP3T2T2T2,						texCoord3,	glm::vec2, 2, VertexP3T2T2)
VertexDef(VertexP3T2T2T2N3,						normal,		glm::vec3, 3, VertexP3T2T2T2)
VertexDef(VertexP3T2T2C3,						color,		glm::vec3, 3, VertexP3T2T2)
VertexDef(VertexP3T2T2C3N3,						normal,		glm::vec3, 3, VertexP3T2T2C3)
VertexDef(VertexP3T2C4,							color,		glm::vec4, 4, VertexP3T2)
VertexDef(VertexP2C4,							color,		glm::vec4, 4, VertexP2)
VertexDef(VertexP3C4,							color,		glm::vec4, 4, VertexP3)
VertexDef(VertexP3T2C3,							color,		glm::vec3, 3, VertexP3T2)
VertexDef(VertexP3T2C1,							filler,			int, 1, VertexP3T2)
VertexDef(VertexP2T2C4,							color,		glm::vec4, 4, VertexP2T2)
VertexDef(VertexP2C4T2,							tex1,		glm::vec2, 2, VertexP2C4)
VertexDef(VertexP2C4T2T2,						tex2,		glm::vec2, 2, VertexP2C4T2)
VertexDef(VertexP2C4T2T2F1,						_size,		float, 1, VertexP2C4T2T2)
VertexDef(VertexP3N3C4,							color,		glm::vec4, 4, VertexP3N3)

#ifndef BLIB_IOS
VertexDefi(VertexP3T2N3B4,						boneIds,	glm::ivec4,4, VertexP3T2N3)
VertexDef(VertexP3T2N3B4B4,						boneWeights,glm::vec4,4, VertexP3T2N3B4)
#endif
/*

	void VertexP2::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
	{
#ifdef __glew_h__
		if (!glEnableVertexAttribArray) //wtf
			glewInit();
#endif
		int tmpIndex = 0;
		if(!index)
		index = &tmpIndex;
		Vertex::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);
		if (!enabledVertexAttributes[*index] && glEnableVertexAttribArray)
		glEnableVertexAttribArray(*index);
		enabledVertexAttributes[*index] = true;

	//	int parentSize = Vertex::size();

		if (glVertexAttribPointer)
			glVertexAttribPointer((*index)++, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset));

		if(index == &tmpIndex)
		{
			for(int i = *index; i < 10; i++)
				if(enabledVertexAttributes[i])
				{
					enabledVertexAttributes[i] = false;
//					if (glDisableVertexAttribArray)
						glDisableVertexAttribArray(i);
				}
		}
	}
	int VertexP2::size() { return Vertex::size() + 2*sizeof(GL_FLOAT); }\

		
	void VertexP3::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
	{
#ifdef __glew_h__
		if (!glEnableVertexAttribArray) //wtf
			glewInit();
#endif
		int tmpIndex = 0;
		if (!index)
			index = &tmpIndex;
		Vertex::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);
		if (!enabledVertexAttributes[*index] && glEnableVertexAttribArray)
			glEnableVertexAttribArray(*index);
		enabledVertexAttributes[*index] = true;

		//	int parentSize = Vertex::size();

		//if (glVertexAttribPointer)
			glVertexAttribPointer((*index)++, 3, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset));

		if (index == &tmpIndex)
		{
			for (int i = *index; i < 10; i++)
				if (enabledVertexAttributes[i])
				{
					enabledVertexAttributes[i] = false;
					//					if (glDisableVertexAttribArray)
					glDisableVertexAttribArray(i);
				}
		}
	}
	int VertexP3::size() { return Vertex::size() + 3 * sizeof(GL_FLOAT); }*/

}
#include "Vertex.h"


#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif


namespace blib
{

VertexDef(VertexP3,								position,	glm::vec3, 3, Vertex)
//VertexDef(VertexP2,								position,	glm::vec2, 2, Vertex)
VertexDef(VertexP3T2,							texCoord,	glm::vec2, 2, VertexP3)
VertexDef(VertexP2T2,							texCoord,	glm::vec2, 2, VertexP2)
VertexDef(VertexP2T2T2,							texCoord2,	glm::vec2, 2, VertexP2T2)
VertexDef(VertexP2T3,							texCoord,	glm::vec3, 3, VertexP2)
VertexDef(VertexP2T3T2,							texCoord2,	glm::vec2, 2, VertexP2T3)
VertexDef(VertexP3T2C4,							color,		glm::vec4, 4, VertexP3T2)
VertexDef(VertexP2C4,							color,		glm::vec4, 4, VertexP2)
VertexDef(VertexP3T2C3,							color,		glm::vec3, 3, VertexP3T2)
VertexDef(VertexP3T2C1,							filler,			int, 1, VertexP3T2)
VertexDef(VertexP2T2C4,							color,		glm::vec4, 4, VertexP2T2)
VertexDef(VertexP2C4T2,							tex1,		glm::vec2, 2, VertexP2C4)
VertexDef(VertexP2C4T2T2,						tex2,		glm::vec2, 2, VertexP2C4T2)
VertexDef(VertexP2C4T2T2F1,						_size,		float, 1, VertexP2C4T2T2)



	void VertexP2::setAttribPointers(bool enabledVertexAttributes[10], void* offset, int *index, int totalSize) \
	{
		int tmpIndex = 0;
		if(!index)
		index = &tmpIndex;
		Vertex::setAttribPointers(enabledVertexAttributes, offset, index, totalSize);
		if(!enabledVertexAttributes[*index])
		glEnableVertexAttribArray(*index);
		enabledVertexAttributes[*index] = true;
		glVertexAttribPointer((*index)++, 2, GL_FLOAT, GL_FALSE, totalSize, (void*)((char*)offset + Vertex::size()));
		if(index == &tmpIndex)
		{
			for(int i = *index; i < 10; i++)
				if(enabledVertexAttributes[i])
				{
					enabledVertexAttributes[i] = false;
					glDisableVertexAttribArray(i);
				}
		}
	}
	int VertexP2::size() { return Vertex::size() + 2*sizeof(GL_FLOAT); }\

}
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
VertexDef(VertexP2,								position,	glm::vec2, 2, Vertex)
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
VertexDef(VertexP2C4T2T2F1,							rotation,	float, 1, VertexP2C4T2T2)
VertexDef(VertexP2C4T2T2F1F1,						_size,		float, 1, VertexP2C4T2T2F1)


}
#include "Quad.h"

namespace blib
{
	namespace math
	{
		Quad3::Quad3(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &v4)
		{
			points[0] = v1;
			points[1] = v2;
			points[2] = v3;
			points[3] = v4;
		}
	}
}
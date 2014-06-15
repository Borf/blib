#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{

		class Plane
		{
		public:
			glm::vec3 normal;

			float D;
		};
	}
}
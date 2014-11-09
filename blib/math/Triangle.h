#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Triangle2
		{
			glm::vec2 v1, v2, v3;
		public:

			Triangle2(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);


			glm::vec2 getInCenter() const;

		};

		class Triangle3
		{
		public:
			glm::vec3 v1, v2, v3;

			Triangle3(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

		};

	}
}
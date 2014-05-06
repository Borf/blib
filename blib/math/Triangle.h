#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Triangle
		{
			glm::vec2 v1, v2, v3;
		public:

			Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);


			glm::vec2 getInCenter() const;

		};
	}
}
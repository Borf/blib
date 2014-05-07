#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{

		class HermiteCurve
		{
		public:
			glm::vec2 p0;
			glm::vec2 p1;

			glm::vec2 m0;
			glm::vec2 m1;


			HermiteCurve(glm::vec2 pos0, glm::vec2 dir0, glm::vec2 pos1, glm::vec2 dir1);

			glm::vec2 getPoint(float t) const;

		};
	}
}
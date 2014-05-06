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


			HermiteCurve(glm::vec2 pos0, glm::vec2 dir0, glm::vec2 pos1, glm::vec2 dir1)
			{
				this->p0 = pos0;
				this->p1 = pos1;
				this->m0 = dir0;
				this->m1 = dir1;
			}

			glm::vec2 getPoint(float t) const
			{
				return (2 * t*t*t - 3 * t*t + 1)*p0 + (t*t*t - 2 * t*t + t)*m0 + (-2 * t*t*t + 3 * t*t)*p1 + (t*t*t - t*t)*m1;
			}

		};
	}
}
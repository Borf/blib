#pragma once
#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{

		class Line
		{
		public:
			glm::vec2 p1;
			glm::vec2 p2;

			Line(glm::vec2 p1, glm::vec2 p2);

			bool intersects(const Line &other) const;
			bool intersects(const Line &other, glm::vec2 &point) const;

		};

	}
}
#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Line;

		class Polygon : public std::vector<glm::vec2>
		{
		public:
			bool contains(glm::vec2 point);
			bool intersects(const Line& line);
			bool intersects(const Line& line, glm::vec2 &point, glm::vec2 &normal);
		};

	}
}

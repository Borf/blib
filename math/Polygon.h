#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <blib/gl/LineBatch.h>

namespace blib
{
	namespace math
	{
		class Line;

		class Polygon : public std::vector<glm::vec2>, public blib::gl::ILineDrawable
		{
		public:
			bool contains(glm::vec2 point);
			bool intersects(const Line& line);
			bool intersects(const Line& line, glm::vec2 &point, Line &collidedLine);

			glm::vec2 normal(int index);


			void buildLines();
		};

	}
}

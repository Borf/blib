#pragma once
#include <glm/glm.hpp>
#include <blib/LineBatch.h>

namespace blib
{
	namespace math
	{

		class Line : public blib::ILineDrawable
		{
		public:
			glm::vec2 p1;
			glm::vec2 p2;

			Line();
			Line(glm::vec2 p1, glm::vec2 p2);


			bool side(const glm::vec2 &point) const;
			bool intersects(const Line &other) const;
			bool intersects(const Line &other, glm::vec2 &point) const;
			glm::vec2 project(const glm::vec2& point) const;

			glm::vec2 normal();
			void buildLines();

		};

	}
}
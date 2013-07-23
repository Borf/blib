#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Rectangle
		{
		public:
			Rectangle(float x, float y, float width, float height);
			Rectangle(glm::vec2 topleft, glm::vec2 bottomright);

			glm::vec2 topleft;
			glm::vec2 bottomright;

			inline glm::vec2 center() const { return (topleft + bottomright) * 0.5f; };

			inline glm::vec2 size() const { return bottomright - topleft; };

			inline float width() const { return bottomright.x - topleft.x; };
			inline float height() const { return bottomright.y - topleft.y; };
		};
	}
}
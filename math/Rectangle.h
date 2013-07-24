#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Circle;
		class Line;
		class Ray;
		class Triangle;

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


			bool contains(glm::vec2 point);
			bool intersect(const blib::math::Rectangle &other);
			bool intersect(const blib::math::Circle &other);
			bool intersect(const blib::math::Line &other);
			bool intersect(const blib::math::Ray &other);
			bool intersect(const blib::math::Triangle &other);
		};
	}
}
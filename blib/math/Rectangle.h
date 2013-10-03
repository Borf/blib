#pragma once

#include <glm/glm.hpp>
#include <blib/IDrawableLine.h>
#include <vector>

#include <clipper/clipper.hpp>

namespace p2t { struct Point; }
namespace blib
{
	namespace math
	{
		class Circle;
		class Line;
		class Ray;
		class Triangle;

		class Rectangle : public blib::IDrawableLine
		{
		public:
			Rectangle(float x, float y, float width, float height);
			Rectangle(glm::vec2 topleft, float width, float height);
			Rectangle(glm::vec2 topleft, int width, int height);
			Rectangle(glm::vec2 topleft, glm::vec2 bottomright);

			glm::vec2 topleft;
			glm::vec2 bottomright;

			inline glm::vec2 center() const { return (topleft + bottomright) * 0.5f; };

			inline glm::vec2 size() const { return bottomright - topleft; };

			inline float width() const { return bottomright.x - topleft.x; };
			inline float height() const { return bottomright.y - topleft.y; };


			bool contains(const glm::vec2 &point) const;
			bool intersect(const blib::math::Rectangle &other) const;
			bool intersect(const blib::math::Circle &other) const;
			bool intersect(const blib::math::Line &other) const;
			bool intersect(const blib::math::Ray &other) const;
			bool intersect(const blib::math::Triangle &other) const;

			void inflate(float x, float y);

			void buildLines();
			ClipperLib::Polygon toClipperPolygon() const;
			std::vector<p2t::Point*> toP2TPolygon() const;
		};
	}
}
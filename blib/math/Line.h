#pragma once
#include <glm/glm.hpp>
#include <blib/IDrawableLine.h>

namespace blib
{
	namespace math
	{

		class Line : public blib::IDrawableLine
		{
		public:
			glm::vec2 p1;
			glm::vec2 p2;

			Line();
			Line(glm::vec2 p1, glm::vec2 p2);


			bool side(const glm::vec2 &point) const;
			bool intersects(const Line &other) const;
			bool intersects(const Line &other, glm::vec2 &point) const;
			bool intersectsAsRays(const Line &other, glm::vec2 &point) const;
			glm::vec2 project(const glm::vec2& point) const;
			float projectFactor(const glm::vec2& point) const;

			glm::vec2 normal() const;
			void buildLines();
			float length() const;
			float angle() const ;
			glm::vec2 mix(float f) const;
		};

	}
}
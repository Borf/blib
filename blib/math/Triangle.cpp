#include "Triangle.h" 
#include <poly2tri/poly2tri.h>
#include <blib/math/Line.h>

namespace blib
{
	namespace math
	{

		Triangle2::Triangle2(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
		{
			this->v1 = v1;
			this->v2 = v2;
			this->v3 = v3;
		}

		Triangle2::Triangle2(p2t::Triangle* t)
		{
			this->v1 = glm::vec2(t->GetPoint(0)->x, t->GetPoint(0)->y);
			this->v2 = glm::vec2(t->GetPoint(1)->x, t->GetPoint(1)->y);
			this->v3 = glm::vec2(t->GetPoint(2)->x, t->GetPoint(2)->y);
		}



		glm::vec2 Triangle2::getInCenter() const
		{
			float a = glm::length(v2 - v3);
			float b = glm::length(v1 - v3);
			float c = glm::length(v1 - v2);
			return (a * v1 + b * v2 + c * v3) / (a + b + c);
		}

		glm::vec2 Triangle2::getCenter() const
		{
			return (v1 + v2 + v3) / 3.0f;
		}

		void Triangle2::buildLines()
		{
			lines.push_back(blib::IDrawableLine::LinePart(v1, v2));
			lines.push_back(blib::IDrawableLine::LinePart(v2, v3));
			lines.push_back(blib::IDrawableLine::LinePart(v3, v1));
		}

		bool Triangle2::contains(const glm::vec2 &p) const
		{
			// Compute vectors        
			glm::vec2 v0 = this->v3 - this->v1;
			glm::vec2 v1 = this->v2 - this->v1;
			glm::vec2 v2 = p - this->v1;

				// Compute dot products
			float dot00 = glm::dot(v0, v0);
			float dot01 = glm::dot(v0, v1);
			float dot02 = glm::dot(v0, v2);
			float dot11 = glm::dot(v1, v1);
			float dot12 = glm::dot(v1, v2);

				// Compute barycentric coordinates
			float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
			float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
			float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

			// Check if point is in triangle
			return (u >= 0) && (v >= 0) && (u + v < 1);
		}


		Triangle3::Triangle3(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
		{
			this->v1 = v1;
			this->v2 = v2;
			this->v3 = v3;

		}

	}
}
#include "Line.h"

namespace blib
{
	namespace math
	{
		Line::Line()
		{

		}
		Line::Line( glm::vec2 p1, glm::vec2 p2 )
		{
			this->p1 = p1;
			this->p2 = p2;
		}

		bool Line::intersects( const Line &other ) const
		{
			glm::vec2 point;
			return intersects(other, point);
		}



		bool Line::intersects( const Line &other, glm::vec2 &point ) const
		{
			float div = ((other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y));
			float fac1 = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x))/div;
			if(fac1 < 0 || fac1 > 1 || std::signbit(fac1) )
				return false;
			float fac2 = ((p2.x - p1.x) * (p1.y - other.p1.y) - (p2.y - p1.y) * (p1.x - other.p1.x)) / div;
			if (fac2 < 0 || fac2 > 1 || std::signbit(fac2))
				return false;

			point = p1 + fac1 * (p2 - p1);
			return true;
		}

		bool Line::intersectsAsRays(const Line &other, glm::vec2 &point) const
		{
			float div = ((other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y));
			float fac1 = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x)) / div;
			point = p1 + fac1 * (p2 - p1);
			return fabs(div) > 0.001;
		}

		glm::vec2 Line::normal() const
		{
			return glm::normalize(glm::vec2(p2.y - p1.y, -(p2.x - p1.x)));
		}

		void Line::buildLines()
		{
			lines.push_back(blib::IDrawableLine::LinePart(p1, p2));
		}

		glm::vec2 Line::project( const glm::vec2& point ) const
		{
/*			Vector2 closestPointBetween(Vector2 a, Vector2 b, Vector2 p)
			{
				Vector2 AB = b - a;
				float ab2 = AB.LengthSquared();
				Vector2 AP = p - a;
				float ap_dot_ab = AP.X * AB.X + AP.Y * AB.Y;
				float t = ap_dot_ab / ab2;
				if (t < 0)
					t = 0;
				if (t > 1)
					t = 1;
				return a + AB * t;
			}	*/

			glm::vec2 AB = p2 - p1;
			float ab2 = glm::dot(AB, AB);
			glm::vec2 AP(point - p1);
			float ap_dot_ab = glm::dot(AP, AB);
			float t = glm::clamp(ap_dot_ab / ab2, 0.0f, 1.0f);
			return p1 + AB * t;
		}
		float Line::projectFactor( const glm::vec2& point ) const
		{
			glm::vec2 AB = p2 - p1;
			float ab2 = glm::dot(AB, AB);
			glm::vec2 AP(point - p1);
			float ap_dot_ab = glm::dot(AP, AB);
			return ap_dot_ab / ab2;
		}

		bool Line::side( const glm::vec2 &point ) const
		{
			return ((p2.x - p1.x)*(point.y - p1.y) - (p2.y - p1.y)*(point.x - p1.x)) > 0;
		}

		float Line::length() const
		{
			return glm::distance(p1, p2);
		}

		float Line::angle() const
		{
			return atan2(p2.y - p1.y, p2.x - p1.x);
		}

		glm::vec2 Line::mix(float fac) const
		{
			return glm::mix(p1, p2, fac);
		}

	}
}
#pragma once

#include "Line.h"

namespace blib
{
	namespace math
	{
		Line::Line( glm::vec2 p1, glm::vec2 p2 )
		{
			this->p1 = p1;
			this->p2 = p2;
		}

		bool Line::intersects( const Line &other ) const
		{
			glm::vec2 point;
			return intersects(other, point);

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
		}



		bool Line::intersects( const Line &other, glm::vec2 &point ) const
		{
			float div = ((other.p2.y - other.p1.y)*(p2.x - p1.x) - (other.p2.x - other.p1.x)*(p2.y - p1.y));
			float fac1 = ((other.p2.x - other.p1.x)*(p1.y - other.p1.y) - (other.p2.y - other.p1.y)*(p1.x - other.p1.x))/div;
			if(fac1 < 0 || fac1 > 1)
				return false;
			float fac2 = ((p2.x - p1.x) * (p1.y - other.p1.y) - (p2.y - p1.y) * (p1.x - other.p1.x)) / div;
			if(fac2 < 0 || fac2 > 1)
				return false;

			return true;
		}

	}
}
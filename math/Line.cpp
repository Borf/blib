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
	/*		public Vector2 LineLineIntersect(Vector2 v1, Vector2 v2, Vector2 v3, Vector2 v4)
			{
				float u = LineLineIntersectFac(v1, v2, v3, v4);
				return v1 + u * (v2 - v1);
			}

			//TODO: move to physics / collision class
			Vector2 closestPointBetween(Vector2 a, Vector2 b, Vector2 p)
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

		}

	}
}
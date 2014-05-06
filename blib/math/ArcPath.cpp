#include "ArcPath.h"
#include <blib/Util.h>

#define _USE_MATH_DEFINES
#include <math.h>


namespace blib
{
	namespace math
	{


		ArcPath::ArcPath(const glm::vec2 &begin, const glm::vec2 &direction, const glm::vec2 &end)
		{
			this->begin = begin;
			this->end = end;
			this->begindirection = glm::normalize(direction);

			glm::vec2 support = glm::normalize(direction);

			glm::vec2 v = end - begin;
			glm::vec2 n1 = glm::vec2(support.y, -support.x);
			glm::vec2 midpoint = begin + v * 0.5f;
			float curvature = 2 * glm::dot(v, n1) / (v.x*v.x + v.y*v.y);

			radius = 1 / curvature;
			origin = begin + n1 * radius;
			sign_t = glm::dot(v, support) > 0;
			sign_n = glm::dot(v, n1) > 0;

			glm::vec2 n2 = glm::normalize(origin - end) * (sign_n ? -1.0f : 1.0f);

			enddirection = glm::vec2();

		}

		void ArcPath::buildLines()
		{
			float r1 = atan2(begin.y - origin.y, begin.x - origin.x);
			float r2 = atan2(end.y - origin.y, end.x - origin.x);

			if (radius < 0 && !sign_n)
			{
				r1 -= M_PI;
				r2 -= M_PI;
			}
			else if (radius > 0 && sign_n && r2 > 0)
			{
				while (r1 < 0)
					r1 += 2 * M_PI;
			}


			float inc = M_PI / (r1 > r2 ? -fabs(radius) : fabs(radius)) * 1.0f;

			float last;
			for (float f = r1; r1 < r2 ? f < r2 : f > r2; f += inc)
			{
				lines.push_back(LinePart(origin + radius * blib::util::fromAngle(f), origin + radius * blib::util::fromAngle(f+inc)));
				last = f;
			}
			lines.push_back(LinePart(origin + radius * blib::util::fromAngle(last), origin + radius * blib::util::fromAngle(r2)));


		}

		void ArcPath::offset(float f)
		{
			radius += f;
		}

	}
}
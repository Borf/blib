#include "ArcPath.h"
#include <blib/Util.h>
#include <blib/util/Log.h>
#include <glm/glm.hpp>

using blib::util::Log;

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

			if (fabs(curvature) > 0.00001)
			{
				glm::vec2 n2 = glm::normalize(origin - end) * (sign_n ? -1.0f : 1.0f);
				enddirection = glm::vec2(-n2.y, n2.x);
			}
			else
				enddirection = begindirection;
			offset = 0;

		}


		float angleDist(float a1, float a2)
		{
			float dist = a1 - a2;
			if (dist > 2 * M_PI)
				dist -= 2 * M_PI;
			if (dist < 0)
				dist += 2 * M_PI;
			return dist;
		}

		void ArcPath::buildLines()
		{
			float rad = fabs(radius+offset);

			if (rad < 400) //TODO: nasty condition here... to protect from 'straight lines'
			{
				float r1 = atan2(begin.y - origin.y, begin.x - origin.x);
				float r2 = atan2(end.y - origin.y, end.x - origin.x);

				float inc = M_PI / (5 + glm::min(100.0f, glm::abs(radius+offset)));
				if (sign_n)
					inc = -inc;

				//todo: fix condition here
				float last = r1;
				for (float f = r1; fabs(angleDist(f + inc, r2)) > fabs(inc); f += inc)
				{
					if (f > M_PI)
						f -= M_PI * 2;
					if (f < -M_PI)
						f += M_PI * 2;

					lines.push_back(LinePart(origin + rad * blib::util::fromAngle(f), origin + rad * blib::util::fromAngle(f + inc)));
					last = f + inc;
				}
				if (!lines.empty())
				{
					//	lines.pop_back();
					lines.push_back(LinePart(origin + rad * blib::util::fromAngle(last), origin + rad * blib::util::fromAngle(r2)));
				}
				else
					lines.push_back(LinePart(origin + rad * blib::util::fromAngle(r1), origin + rad * blib::util::fromAngle(r2)));
			}
			else
			{
				lines.push_back(LinePart(begin, end));
			}
		}

		float ArcPath::length() const
		{
			if (fabs(radius + offset) < 400.0f)
			{
				float a1 = atan2(begin.y - origin.y, begin.x - origin.x);
				float a2 = atan2(end.y - origin.y, end.x - origin.x);
				// NOTE: sign_t means that the angular span exceeds 180 deg.
				float da = fabs(a2 - a1);
				if (sign_t == (da > M_PI)) {
					da = 2 * M_PI - da;
				}
				return fabs(da * (radius + offset));
			}
			else
			{
				return glm::length(begin - end);
			}
		}

		void ArcPath::setOffset(float offset)
		{
			this->offset = offset;
		}

		glm::vec2 ArcPath::getPoint(float f) const
		{
			float rad = fabs(radius+offset);

			if (rad < 400) //TODO: nasty condition here... to protect from 'straight lines'
			{
				float r1 = atan2(begin.y - origin.y, begin.x - origin.x);
				float r2 = atan2(end.y - origin.y, end.x - origin.x);

				float inc = 0;
				if (!sign_n)
					inc = angleDist(r2, r1) * f;
				else
					inc = angleDist(r1, r2) * f;

				if (sign_n)
					inc = -inc;

				//todo: fix condition here
				float angle = r1 + inc;
				return origin + rad * blib::util::fromAngle(angle);
			}
			else
			{
				glm::vec2 beginNormal(begindirection.y, -begindirection.x);
				return begin + (sign_t ? -offset : offset) * glm::normalize(beginNormal) + f * (end - begin);
			}


		}

	}
}
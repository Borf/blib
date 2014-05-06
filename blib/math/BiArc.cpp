#include "BiArc.h"

#include "Line.h"
#include "Triangle.h"
#include <blib/Math.h>
#include <blib/Util.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{
	namespace math
	{
		BiArc::BiArc(const glm::vec2 &begin, const glm::vec2 &beginDirection, const glm::vec2 &end, const glm::vec2 &endDirection)
		{
			glm::vec2 v1 = end - begin;
			glm::vec2 v2 = (end + endDirection*-100.0f) - (begin + beginDirection*100.0f);

			glm::vec2 m1 = begin + 0.5f * v1;
			glm::vec2 m2 = begin + 100.0f * beginDirection + 0.5f * v2;

			glm::vec2 intersection;
			Line l2(m2, m2 + glm::vec2(v2.y, -v2.x));
			Line l1(m1, m1 + glm::vec2(v1.y, -v1.x));
			
			if(!l1.intersectsAsRays(l2, intersection))
			{ 
				intersection = (begin + end) * 0.5f;
			}
			float radius = glm::length(begin - intersection);

			midPointCenter = intersection;
			midPointRadius = radius;


			midPoint = intersection + radius * blib::util::fromAngle(0.5 * M_PI);

//			else
//				midPoint = (begin + end) / 2.0f;

			p1 = ArcPath(begin, beginDirection, midPoint);
			p2 = ArcPath(end, endDirection, midPoint);




			if (p1.lines.empty())
			{
				p1.buildLines();
				p2.buildLines();
			}
			lines.clear();
			lines.insert(lines.end(), p1.lines.begin(), p1.lines.end());
			lines.insert(lines.end(), p2.lines.begin(), p2.lines.end());


			lines.push_back(LinePart(midPoint + glm::vec2(-2, -2), midPoint + glm::vec2(2, 2)));
			lines.push_back(LinePart(midPoint + glm::vec2(-2, 2), midPoint + glm::vec2(2, -2)));


			for (float i = 0; i < 2 * M_PI; i += M_PI / 40.0f)
			{
				float ii = i + M_PI / 40.0f;
				lines.push_back(LinePart(midPointCenter + midPointRadius * blib::util::fromAngle(i), midPointCenter + midPointRadius * blib::util::fromAngle(ii)));
			}


			lines.insert(lines.end(), l1.getLines().begin(), l1.getLines().end());
			lines.insert(lines.end(), l2.getLines().begin(), l2.getLines().end());

		}

		void BiArc::buildLines()
		{


		}

	}
}

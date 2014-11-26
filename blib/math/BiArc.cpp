#include "BiArc.h"

#include "Line.h"
#include <blib/Math.h>
#include <blib/Util.h>
#include <blib/util/Log.h>

using blib::util::Log;

#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{
	namespace math
	{
		BiArc::BiArc(const glm::vec2 &begin, const glm::vec2 &beginDirection, const glm::vec2 &end, const glm::vec2 &endDirection)
		{
			float wtf = 100.0f;

			glm::vec2 v1 = end - begin;
			glm::vec2 v2 = (end - wtf * endDirection) - (begin + wtf * beginDirection);

			glm::vec2 m1 = begin + 0.5f * v1;
			glm::vec2 m2 = begin + wtf * beginDirection + 0.5f * v2;

			glm::vec2 intersection;
			Line l1(m1, m1 + glm::vec2(v1.y, -v1.x));
			Line l2(m2, m2 + glm::vec2(v2.y, -v2.x));

			if(!l1.intersectsAsRays(l2, intersection))
			{ 
				midPoint = (begin + end) * 0.5f;
			}
			else
			{
				float radius = glm::length(begin - intersection);

				midPointCenter = intersection;
				midPointRadius = radius;

				/*			int best = 0;
							float bestLen = 999999999999;
							for (int i = 0; i < 30; i++)
							{
							midPoint = intersection + radius * blib::util::fromAngle(2 * M_PI * i / 30);

							p1 = ArcPath(begin, beginDirection, midPoint);
							p2 = ArcPath(end, endDirection, midPoint);

							if (p1.length() + p2.length() < bestLen)
							{
							bestLen = p1.length() + p2.length();
							best = i;
							}
							}
							midPoint = intersection + radius * blib::util::fromAngle(2 * M_PI * best / 30);*/


				float a1 = atan2(begin.y - intersection.y, begin.x - intersection.x);
				float a2 = atan2(end.y - intersection.y, end.x - intersection.x);
				float newAngle1 = (a1 + a2) / 2;
                float newAngle2 = (float)(newAngle1 + blib::math::pif);

				float len1 = ArcPath(begin, beginDirection, intersection + radius * blib::util::fromAngle(newAngle1)).length() +
					ArcPath(end, endDirection, intersection + radius * blib::util::fromAngle(newAngle1)).length();

				float len2 = ArcPath(begin, beginDirection, intersection + radius * blib::util::fromAngle(newAngle2)).length() +
		 					ArcPath(end, endDirection, intersection + radius * blib::util::fromAngle(newAngle2)).length();

				
				midPoint = intersection + radius * blib::util::fromAngle(len1 < len2 ? newAngle1 : newAngle2);
			}


			p1 = ArcPath(begin, beginDirection, midPoint);
			p2 = ArcPath(end, endDirection, midPoint);
			p2 = ArcPath(midPoint, p2.enddirection, end);

			


			if (glm::length(p1.enddirection + p2.enddirection) > 0.01f)
			{
				Log::out << "OOHAOEIURHA" << Log::newline;
			}

//			buildLines();


		//	lines.push_back(LinePart(midPoint + glm::vec2(-2, -2), midPoint + glm::vec2(2, 2)));
		//	lines.push_back(LinePart(midPoint + glm::vec2(-2, 2), midPoint + glm::vec2(2, -2)));


			
		/*	for (float i = 0; i < 2 * M_PI; i += M_PI / 40.0f)
			{
				float ii = i + M_PI / 40.0f;
				lines.push_back(LinePart(midPointCenter + midPointRadius * blib::util::fromAngle(i), midPointCenter + midPointRadius * blib::util::fromAngle(ii)));
			}*/





		//	lines.insert(lines.end(), l1.getLines().begin(), l1.getLines().end());
			//lines.insert(lines.end(), l2.getLines().begin(), l2.getLines().end());

		}


		void BiArc::buildLines()
		{
			if (p1.lines.empty())
			{
				p1.buildLines();
				p2.buildLines();
			}
			lines.clear();
			lines.insert(lines.end(), p1.lines.begin(), p1.lines.end());
			lines.insert(lines.end(), p2.lines.begin(), p2.lines.end());
		}

		void BiArc::setOffset(float offset)
		{
			p1.setOffset(offset);
			p2.setOffset(offset);
		}

		glm::vec2 BiArc::getPoint(float f) const
		{
			float len1 = p1.length();
			float len2 = p2.length();
			float totalLen = len1 + len2;
            //float pos = f * totalLen;

			float midPoint = len1 / totalLen;

			if (f < midPoint)
			{
				return p1.getPoint(f / midPoint);
			}
			else
				return p2.getPoint((f - midPoint) / (1 - midPoint));
		}

		glm::vec2 BiArc::getPointLinear(float f) const
		{
			if (f < 0.5)
			{
				return p1.getPoint(f * 2);
			}
			else
				return p2.getPoint((f - 0.5f) * 2);
		}

	}
}

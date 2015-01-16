#pragma once

#include <blib/IDrawableLine.h>
#include <blib/Math.h>

namespace blib
{
	namespace math
	{
		class Circle : public IDrawableLine
		{
			glm::vec2 center;
			float radius;
		public:
			Circle(const glm::vec2 &center, float radius)
			{
				this->center = center;
				this->radius = radius;
			}

			virtual void buildLines() override
			{
				float inc = blib::math::pif / 5;
				for (float d = 0; d < 2 * blib::math::pi; d += inc)
					lines.push_back(blib::IDrawableLine::LinePart(center + radius * blib::util::fromAngle(d), center + radius * blib::util::fromAngle(d+inc)));
			}

		};
	}
}
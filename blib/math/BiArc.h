#pragma once

#include <glm/glm.hpp>
#include <blib/math/ArcPath.h>
#include <blib/IDrawableLine.h>

namespace blib
{
	namespace math
	{
		class BiArc : public IDrawableLine
		{
			ArcPath p1;
			ArcPath p2;

			glm::vec2 midPoint;

			glm::vec2 midPointCenter;
			float midPointRadius;
		public:
			BiArc() {};

			BiArc(const glm::vec2 &begin, const glm::vec2 &beginDirection, const glm::vec2 &end, const glm::vec2 &endDirection);
			void setOffset(float offset);

			glm::vec2 getPoint(float f) const;
			glm::vec2 getPointLinear(float f) const;

			void buildLines();
			inline float length() const { return p1.length() + p2.length(); };
		};
	}
}
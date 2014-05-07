#pragma once

#include <glm/glm.hpp>
#include <blib/IDrawableLine.h>

namespace blib
{
	namespace math
	{
		class ArcPath : public blib::IDrawableLine
		{
			bool sign_t;
			bool sign_n;
		public:
			glm::vec2 begin;
			glm::vec2 end;
			glm::vec2 begindirection;
			glm::vec2 enddirection;

			glm::vec2 origin;
			float radius;
			float offset;


			ArcPath(const glm::vec2 &begin, const glm::vec2 &direction, const glm::vec2 &end);
			ArcPath() {};


			float length() const;
			glm::vec2 getPoint(float f) const;

			void setOffset(float f);

			void buildLines();
		};


	}
}
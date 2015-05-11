#pragma once

#include <glm/glm.hpp>
#include <blib/IDrawableLine.h>

namespace p2t { class Triangle; }

namespace blib
{
	namespace math
	{
		class Triangle2 : public IDrawableLine
		{
		public:
			glm::vec2 v1, v2, v3;


			Triangle2(p2t::Triangle* t);
			Triangle2(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3);


			glm::vec2 getInCenter() const;
			glm::vec2 getCenter() const;
			bool contains(const glm::vec2&) const;


			virtual void buildLines() override;

		};

		class Triangle3
		{
		public:
			glm::vec3 v1, v2, v3;

			Triangle3(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

		};

	}
}
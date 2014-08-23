#pragma once

#include <glm/glm.hpp>

namespace blib
{
	namespace math
	{
		class Ray;

		class Quad3
		{
			glm::vec3 points[4];
		public:
			Quad3(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &v4);

			glm::vec3& operator [](int i) const;
			glm::vec3 normal() const;
			glm::vec4 collides(const Ray& ray) const;
		};
	}
}
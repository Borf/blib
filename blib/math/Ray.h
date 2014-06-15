#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace blib
{
	namespace math
	{
		class Plane;


		class Ray
		{
		public:
			glm::vec3 origin;
			glm::vec3 dir;
			glm::vec3 invDir;
			int sign[3];

			Ray(glm::vec3 origin, glm::vec3 dir);

			void calcSign();


			bool planeIntersection(const Plane &plane, float &t) const;
			bool LineIntersectPolygon(const std::vector<glm::vec3> &vertices, float &t) const;

			Ray operator *(const glm::mat4 &matrix) const;

		};

	}
}

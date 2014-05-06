#include "Triangle.h" 


namespace blib
{
	namespace math
	{

		Triangle::Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
		{
			this->v1 = v1;
			this->v2 = v2;
			this->v3 = v3;
		}



		glm::vec2 Triangle::getInCenter() const
		{
			float a = glm::length(v2 - v3);
			float b = glm::length(v1 - v3);
			float c = glm::length(v1 - v2);
			return (a * v1 + b * v2 + c * v3) / (a + b + c);

		}

	}
}
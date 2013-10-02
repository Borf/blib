#include "Math.h"
#include <stdlib.h>

#include <glm/gtc/matrix_transform.hpp>
#include <Box2D/Box2D.h>

namespace blib
{
	namespace math
	{


		double randomDouble()
		{
			return rand() / (double)RAND_MAX;
		}

		glm::mat4 box2dMatrix( b2Body* body, glm::mat4 matrix )
		{
			matrix = glm::translate(matrix, glm::vec3(body->GetPosition().x, body->GetPosition().y, 0));
			matrix = glm::rotate(matrix, glm::degrees(body->GetAngle()), glm::vec3(0,0,1));
			return matrix;
		}

	}
}
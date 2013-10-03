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

		double randomDouble( float max )
		{
			return randomDouble()*max;
		}

		double randomDouble( float min, float max )
		{
			return min + randomDouble()*(max-min);
		}

		float randomFloat()
		{
			return rand() / (float)RAND_MAX;
		}

		float randomFloat( float max )
		{
			return randomFloat()*max;
		}

		float randomFloat( float min, float max )
		{
			return min + randomFloat()*(max-min);
		}


		glm::mat4 box2dMatrix( b2Body* body, glm::mat4 matrix )
		{
			matrix = glm::translate(matrix, glm::vec3(body->GetPosition().x, body->GetPosition().y, 0));
			matrix = glm::rotate(matrix, glm::degrees(body->GetAngle()), glm::vec3(0,0,1));
			return matrix;
		}

		glm::mat4 easyMatrix( const glm::vec2 &position, glm::mat4 matrix /*= glm::mat4()*/ )
		{
			return glm::translate(matrix, glm::vec3(position, 0));
		}

	}
}
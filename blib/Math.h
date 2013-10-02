#pragma once

#include <glm/glm.hpp>

class b2Body;

namespace blib
{
	namespace math
	{
		double randomDouble();
		double randomDouble(float max);
		double randomDouble(float min, float max);
		float randomFloat();
		float randomFloat(float max);
		float randomFloat(float min, float max);
		glm::mat4 box2dMatrix(b2Body* body, glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation, glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation, float scale, glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation, const glm::vec2 &scale, glm::mat4 matrix = glm::mat4());
	}
}
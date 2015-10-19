#pragma once

#include <glm/glm.hpp>
#include <blib/TextureMap.h>
#include <vector>
class b2Body;

namespace blib
{
	class Texture;

	namespace math
	{
        static const double  pi = 3.14159265358979323846;
        static const float pif = 3.14159265358979323846f;


		class Rectangle;


		double randomDouble();
		double randomDouble(float max);
		double randomDouble(float min, float max);
		float randomFloat();
		float randomFloat(float max);
		float randomFloat(float min, float max);
		float round(float number, int digits);
		glm::mat4 box2dMatrix(b2Body* body, glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position,											const glm::mat4 &matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation,							glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, const glm::vec3 &rotation,				glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation, float scale,			glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const glm::vec2 &position, float rotation, const glm::vec2 &scale, glm::mat4 matrix = glm::mat4());
		glm::mat4 easyMatrix(const Texture* texture, const blib::math::Rectangle &rect);
		glm::mat4 easyMatrix(const Texture* texture, const blib::math::Rectangle &rect, glm::vec2 pivot, float rotation);
		glm::mat4 easyMatrix(const TextureMap::TexInfo* texture, const blib::math::Rectangle &rect);


		glm::vec2 fromAngle(float angle);
		float wrapAngleRad(float angle);
		float wrapAngleDeg(float angle);


		template<class T>
		std::vector<T> permute(const std::vector<T> &values)
		{
			if(values.empty())
				return std::vector<T>();

			std::vector<T> permutation(values.size());

			permutation[0] = values[0];
			for (size_t  i = 1; i < values.size(); i++)
			{
				int j = rand() % (i+1);
				permutation[i] = permutation[j];
				permutation[j] = values[i];
			}
			return permutation;

		}

	}
}

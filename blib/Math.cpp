#include "Math.h"
#include <stdlib.h>

#include <blib/math/Rectangle.h>
#include <blib/Texture.h>
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

		glm::mat4 easyMatrix( const glm::vec2 &position, const glm::mat4& matrix /*= glm::mat4()*/ )
		{
			return glm::translate(matrix, glm::vec3(position, 0));
		}

		glm::mat4 easyMatrix( const glm::vec2 &position, float rotation, glm::mat4 matrix /*= glm::mat4()*/ )
		{
			return glm::rotate(glm::translate(matrix, glm::vec3(position, 0)), rotation, glm::vec3(0,0,1));
		}

		glm::mat4 easyMatrix( const glm::vec2 &position, const glm::vec3 &rotation, glm::mat4 matrix /*= glm::mat4()*/ )
		{
			return glm::translate(glm::rotate(glm::translate(matrix, glm::vec3(position-glm::vec2(rotation), 0)), rotation.z, glm::vec3(0,0,1)), glm::vec3(glm::vec2(rotation),0));
		}


		glm::mat4 easyMatrix( const glm::vec2 &position, float rotation, const glm::vec2 &scale, glm::mat4 matrix /*= glm::mat4()*/ )
		{
			return glm::scale(glm::rotate(glm::translate(matrix, glm::vec3(position, 0)), rotation, glm::vec3(0,0,1)), glm::vec3(scale,1));
		}

		glm::mat4 easyMatrix( const glm::vec2 &position, float rotation, float scale, glm::mat4 matrix /*= glm::mat4()*/ )
		{
			return glm::scale(glm::rotate(glm::translate(matrix, glm::vec3(position, 0)), rotation, glm::vec3(0,0,1)), glm::vec3(scale,scale,1));
		}






		glm::mat4 easyMatrix( const Texture* texture, const Rectangle &rect )
		{
			glm::vec2 scale = rect.size() / glm::vec2(texture->originalWidth,texture->originalHeight);
			return glm::scale(glm::translate(glm::mat4(), glm::vec3(rect.topleft,0)), glm::vec3(scale,1));
		}

		glm::mat4 easyMatrix( const TextureMap::TexInfo* info, const Rectangle &rect )
		{
			glm::vec2 scale = rect.size() / glm::vec2(info->width,info->height);
			return glm::scale(glm::translate(glm::mat4(), glm::vec3(rect.topleft,0)), glm::vec3(scale,1));
		}

		glm::mat4 easyMatrix(const Texture* texture, const Rectangle &rect, glm::vec2 pivot, float rotation)
		{
			glm::vec2 scale = rect.size() / glm::vec2(texture->originalWidth, texture->originalHeight);

			glm::mat4 ret;
			ret = glm::translate(ret, glm::vec3(rect.topleft, 0));
			ret = glm::translate(ret, glm::vec3((pivot - rect.topleft), 0));
			ret = glm::rotate(ret, rotation, glm::vec3(0, 0, 1));
			ret = glm::translate(ret, glm::vec3(-(pivot - rect.topleft), 0));
			ret = glm::scale(ret, glm::vec3(scale, 1));


			return ret;
		}


		glm::vec2 fromAngle(float angle)
		{
			return glm::vec2(cos(angle), sin(angle));
		}

		float wrapAngleRad(float angle)
		{
			while (angle > glm::radians(180.0f))
				angle -= glm::radians(360.0f);
			while (angle < -glm::radians(180.0f))
				angle += glm::radians(360.0f);
			return angle;
		}

		float wrapAngleDeg(float angle)
		{
			while (angle > 180.0f)
				angle -= 360.0f;
			while (angle < -180.0f)
				angle += 360.0f;
			return angle;
		}

		float round( float number, int digits )
		{
			for(int i = 0; i < digits; i++)
				number *= 10;
			number = glm::round(number);
			for(int i = 0; i < digits; i++)
				number /= 10;
			return number;
		}

	}
}

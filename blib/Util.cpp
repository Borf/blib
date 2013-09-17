#define _CRT_SECURE_NO_WARNINGS

#include <blib/util.h>


namespace blib
{
	namespace util
	{
		glm::vec2 fromAngle( float angle )
		{
			return glm::vec2(cos(angle), sin(angle));
		}

		float wrapAngleRad(float angle)
		{
			while(angle > glm::radians(180.0f))
				angle -= glm::radians(360.0f);
			while(angle < -glm::radians(180.0f))
				angle += glm::radians(360.0f);
			return angle;
		}

		float wrapAngleDeg(float angle)
		{
			while(angle > 180.0f)
				angle -= 360.0f;
			while(angle < -180.0f)
				angle += 360.0f;
			return angle;
		}


		std::string toString( int number )
		{
			char buf[32];
			sprintf(buf, "%d", number);
			return buf;
		}


	}
}


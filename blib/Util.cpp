#define _CRT_SECURE_NO_WARNINGS

#include <blib/Util.h>


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

		std::vector<std::string> split( std::string value, std::string seperator )
		{
			std::vector<std::string> ret;
			while(value.find(seperator) != std::string::npos)
			{
				int index = value.find(seperator);
				if(index != 0)
					ret.push_back(value.substr(0, index));
				value = value.substr(index+seperator.length());
			}
			ret.push_back(value);
			return ret;
		}


	}
}

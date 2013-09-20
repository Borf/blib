#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <string.h>

namespace blib
{
	namespace util
	{
		glm::vec2 fromAngle(float angle);
		float wrapAngleRad(float angle);
		float wrapAngleDeg(float angle);

		std::string toString(int number);
		std::vector<std::string> split(std::string toSplit, std::string seperator);

		template<class T>
		T swapByteOrder( T val)
		{
			char* buf = (char*)&val;
			std::swap(buf[0], buf[3]);
			std::swap(buf[1], buf[2]);
			return val;
		}

#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

#define HASH(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))


	}
}
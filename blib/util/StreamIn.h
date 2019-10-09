#pragma once

#include "Stream.h"
#include <string>
#include <glm/glm.hpp>

namespace blib
{
	namespace util
	{
		class StreamIn : public Stream
		{
		public:
			virtual ~StreamIn() {}
			virtual unsigned int read(char* data, int count) = 0;
			virtual char get() = 0;
			virtual bool eof() = 0;


			short readWord();
			unsigned short readUWord();
			short readShort();
			unsigned short readUShort();
			int readInt();
			float readFloat();
			std::string readString(int maxLength, int length = -1);
			std::string readLine();
			glm::vec3 readVec3();
			glm::vec2 readVec2();
		};
	}
}
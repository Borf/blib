#pragma once

#include "Stream.h"
#include <glm/glm.hpp>
#include <string>


namespace blib
{
	namespace util
	{

		class StreamOut : public Stream
		{
		public:
			virtual ~StreamOut() {}
			virtual void write(char* data, int count) = 0;
			virtual void put(char);

			void writeWord(short data);
			void writeShort(short data);
			void writeInt(int data);
			void writeFloat(float data);
			void writeString(const std::string& data, int length);
			void writeLine(const std::string& data);
			void writeVec3(const glm::vec3 &data);
			void writeVec2(const glm::vec2 &data);

		};

		
	}
}
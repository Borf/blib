#include "StreamOut.h"


namespace blib
{
	namespace util
	{

		void StreamOut::writeWord(short data)
		{
			put((data >> 0) & 0xff);
			put((data >> 8) & 0xff);
		}

		void StreamOut::writeUWord(unsigned short data)
		{
			put((data >> 0) & 0xff);
			put((data >> 8) & 0xff);
		}

		void StreamOut::writeShort(short data)
		{
			put((data >> 8) & 0xff);
			put((data >> 0) & 0xff);
		}

		void StreamOut::writeUShort(unsigned short data)
		{
			put((data >> 8) & 0xff);
			put((data >> 0) & 0xff);
		}

		void StreamOut::writeInt(int data)
		{
			write((char*)(&data), 4);
		}

		void StreamOut::writeFloat(float data)
		{
			write((char*)(&data), 4);
		}

		void StreamOut::writeString(const std::string& data, int length)
		{
			write((char*)data.c_str(), data.length());
			for (int i = data.length(); i < length; i++)
				put('\0');
		}

		void StreamOut::writeLine(const std::string& data)
		{
			write((char*)data.c_str(), data.length());
			put('\n');
		}

		void StreamOut::writeVec3(const glm::vec3 &data)
		{
			writeFloat(data.x);
			writeFloat(data.y);
			writeFloat(data.z);
		}

		void StreamOut::writeVec2(const glm::vec2 &data)
		{
			writeFloat(data.x);
			writeFloat(data.y);
		}

		void StreamOut::put(char data)
		{
			write(&data, 1);
		}

	}
}
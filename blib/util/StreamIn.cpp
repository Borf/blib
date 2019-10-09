#include "StreamIn.h"




short blib::util::StreamIn::readShort()
{
	int a = (unsigned char)get();
	int b = (unsigned char)get();
	return a<<8 | b;
}

unsigned short blib::util::StreamIn::readUShort()
{
	int a = (unsigned char)get();
	int b = (unsigned char)get();
	return a << 8 | b;
}


short blib::util::StreamIn::readWord()
{
	int a = (unsigned char)get();
	int b = (unsigned char)get();
	return b<<8 | a;
}

unsigned short blib::util::StreamIn::readUWord()
{
	int a = (unsigned char)get();
	int b = (unsigned char)get();
	return b << 8 | a;
}

float blib::util::StreamIn::readFloat()
{
	float f;
	read((char*)&f, 4);
	return f;
}

std::string blib::util::StreamIn::readString( int maxLength, int length)
{
	char* buf = new char[maxLength];
	read(buf, maxLength);
	std::string ret(buf);
	if (length > -1)
		ret = std::string(buf, length);
	delete[] buf;
	return ret;
}

std::string blib::util::StreamIn::readLine()
{
	char bufje[200] = "";
	int i = 0;
	do
	{
		read(bufje+i, 1);
		i++;
		if(eof())
			break;
	} while(bufje[i-1] != '\n');

	if(i > 0)
		while(i > 0 && (bufje[i-1] == '\n' || bufje[i-1] == '\r'))
			i--;
	return std::string(bufje, i);
}

glm::vec3 blib::util::StreamIn::readVec3()
{
	float x = readFloat();
	float y = readFloat();
	float z = readFloat();
	return glm::vec3(x,y,z);
}

glm::vec2 blib::util::StreamIn::readVec2()
{
	float x = readFloat();
	float y = readFloat();
	return glm::vec2(x,y);
}

int blib::util::StreamIn::readInt()
{
	int i;
	read((char*)&i, 4);
	return i;
}

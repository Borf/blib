#pragma once

#include "Stream.h"


namespace blib
{
	namespace util
	{
		class StreamIn : public Stream
		{
		public:
			virtual unsigned int read(char* data, int count) = 0;
			virtual char get() = 0;
			virtual bool eof() = 0;
		};
	}
}
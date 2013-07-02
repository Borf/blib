#pragma once

#include "Stream.h"


namespace blib
{
	namespace util
	{

		class StreamOut : public Stream
		{
		public:
			virtual void write(char* data, int count) = 0;
			virtual void put(char);
		};
	}
}
#pragma once

#include <blib/util/StreamOut.h>

namespace blib
{
	namespace util
	{
		class StreamOutFile : public StreamOut
		{
		public:
			std::string fileName;
		};
	}
}
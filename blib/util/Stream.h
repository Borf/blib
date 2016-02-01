#pragma once

#include <blib/util/NotCopyable.h>

namespace blib
{
	namespace util
	{
		class Stream : blib::util::NotCopyable
		{
		public:
			virtual ~Stream() {}
		};
	}
}
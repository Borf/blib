#pragma once

namespace blib
{
	namespace util
	{
		class StreamSeekable
		{
		public:
			enum StreamOffset
			{
				BEGIN,
				END,
				CURRENT,
			};
			virtual ~StreamSeekable() {};

			virtual void seek(int offset, StreamOffset offsetTo) = 0;
			virtual unsigned int tell() = 0;
		};
	}
}
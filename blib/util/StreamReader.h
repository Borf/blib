#pragma once

#include <blib/util/StreamIn.h>

namespace blib
{
	namespace util
	{
		class StreamReader : public StreamIn
		{
			StreamIn* baseStream;
		public:
			StreamReader(StreamIn* baseStream) : baseStream(baseStream)
			{
			}

			virtual unsigned int read( char* data, int count )
			{
				return baseStream->read(data, count);
			}

			virtual char get()
			{
				return baseStream->get();
			}

			virtual bool eof()
			{
				return baseStream->eof();
			}


			std::string getLine()
			{
				std::string line = "";
				while(true)
				{
					char c = get();
					if(c == '\r' || c == '\n' || eof())
						break;
					line += c;
				}
				return line;
			}

		};

	}
}


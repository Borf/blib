#pragma once

#include <blib/util/StreamIn.h>
#include <blib/util/StreamSeekable.h>
#include <string>


namespace blib
{
	namespace util
	{
		class StreamInFile : public StreamIn, public StreamSeekable
		{
		protected:
			std::string fileName;
			StreamInFile* innerStream;

			StreamInFile()
			{
				innerStream = NULL;
			}
		public:
			StreamInFile(std::string fileName);
			~StreamInFile()
			{
				if(innerStream)
					delete innerStream;
				innerStream = NULL;
			}

			virtual unsigned int read( char* data, int count )
			{
				return innerStream->read(data, count);
			}
			virtual char get()
			{
				return innerStream->get();
			}
			virtual bool eof()
			{
				return innerStream->eof();
			}

			virtual void seek( int offset, StreamOffset offsetTo )
			{
				innerStream->seek(offset, offsetTo);
			}

			virtual unsigned int tell()
			{
				return innerStream->tell();
			}

		};
	}
}
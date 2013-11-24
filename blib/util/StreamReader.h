#pragma once

#include <blib/util/StreamIn.h>

namespace blib
{
	namespace util
	{
		template<class T>
		class StreamReaderTemplate : public T
		{
		protected:
			T* baseStream;
		public:
			StreamReaderTemplate(T* baseStream) : baseStream(baseStream)
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


		class StreamReader : public StreamReaderTemplate<StreamIn>
		{
		public:
			StreamReader(StreamIn* baseStream) : StreamReaderTemplate<StreamIn>(baseStream) {};
		};

		class StreamReaderFile : public StreamReaderTemplate<StreamInFile>
		{
		public:
			StreamReaderFile(StreamInFile* baseStream) : StreamReaderTemplate<StreamInFile>(baseStream) {};

			virtual unsigned int tell()
			{
				return baseStream->tell();
			}

			virtual void seek( int offset, StreamOffset offsetTo )
			{
				return baseStream->seek(offset, offsetTo);
			}



		};



	}
}

